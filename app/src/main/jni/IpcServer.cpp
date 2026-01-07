#include "IpcServer.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <fcntl.h>
#include <android/log.h>
#include "obfuscate.h"
#include "GameLogic.h" // For g_State access

#define TAG "MLBS_IPC"

static int server_fd = -1;
static std::atomic<bool> server_running(false);
static std::vector<int> clients;
static std::mutex clients_mutex;

// Queue for broadcast messages to avoid blocking game thread
static std::deque<std::string> message_queue;
static std::mutex queue_mutex;
static std::condition_variable queue_cv;

static std::thread server_thread;
static std::thread broadcaster_thread;

void SetNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags != -1) {
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    }
}

void BroadcastData(const std::string& data);

void HandleIncomingCommand(const std::string& cmd) {
    if (cmd.find("cmd:stop") != std::string::npos) {
        std::lock_guard<std::mutex> lock(g_State.stateMutex);
        g_State.roomInfoEnabled = false;
        BroadcastData("{\"type\":\"log\", \"msg\":\"Feature PAUSED by user\"}");
    }
    else if (cmd.find("cmd:start") != std::string::npos) {
        std::lock_guard<std::mutex> lock(g_State.stateMutex);
        g_State.roomInfoEnabled = true;
        BroadcastData("{\"type\":\"log\", \"msg\":\"Feature RESUMED by user\"}");
    }
}

void ClientReaderLoop(int client_fd) {
    char buffer[1024];
    while (server_running) {
        // Simple blocking read (or non-blocking with sleep)
        // Since we are in a thread per client or just one thread, let's use non-blocking + sleep for simplicity
        // to check server_running, or blocking read if we close socket on stop.
        // We set O_NONBLOCK earlier, so read will return -1/EAGAIN.

        ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            std::string cmd(buffer);
            HandleIncomingCommand(cmd);
        } else if (bytes_read == 0) {
            // Disconnected
            break;
        } else {
            // Error or WouldBlock
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            } else {
                break;
            }
        }
    }

    // Cleanup client from list
    close(client_fd);
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        auto it = std::find(clients.begin(), clients.end(), client_fd);
        if (it != clients.end()) {
            clients.erase(it);
        }
    }
}

void HandleClient(int client_fd) {
    SetNonBlocking(client_fd);
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.push_back(client_fd);
    }

    // Spawn a reader thread for this client
    std::thread(ClientReaderLoop, client_fd).detach();
}

void BroadcasterLoop() {
    while (server_running) {
        std::string message;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            queue_cv.wait(lock, [] { return !message_queue.empty() || !server_running; });

            if (!server_running && message_queue.empty()) break;

            if (!message_queue.empty()) {
                message = message_queue.front();
                message_queue.pop_front();
            }
        }

        if (!message.empty()) {
            std::lock_guard<std::mutex> lock(clients_mutex);
            for (auto it = clients.begin(); it != clients.end(); ) {
                int fd = *it;
                std::string msg = message + "\n";
                // MSG_NOSIGNAL prevents SIGPIPE if client disconnected
                // We use non-blocking send indirectly because we set O_NONBLOCK on socket.
                // However, if buffer is full, send might fail with EAGAIN.
                // In that case, we just drop the packet for that client to avoid lagging everyone else.
                ssize_t sent = send(fd, msg.c_str(), msg.length(), MSG_NOSIGNAL);

                if (sent < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
                    // Real error, disconnect logic is handled in Reader Loop or here
                    // If we remove here, Reader Loop might crash or error out.
                    // Let's assume Reader Loop handles disconnect primarily, but if write fails drastically we can remove.
                    // For now, let's keep list management simple. If write fails, we'll likely detect it in read loop eventually.
                    // But to be safe:
                    // close(fd);
                    // it = clients.erase(it);
                    // Actually, let's leave it to Reader Loop to cleanup to avoid double-free race.
                    ++it;
                } else {
                    ++it;
                }
            }
        }
    }
}

void ServerLoop() {
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd < 0) return;

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;

    // Abstract namespace: first byte is null
    // We can obfuscate the name string copy
    const char* obf_name = OBFUSCATE("mlbs_ipc");
    strncpy(addr.sun_path + 1, obf_name, sizeof(addr.sun_path) - 2);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(sa_family_t) + 1 + strlen(obf_name)) < 0) {
        close(server_fd);
        return;
    }

    if (listen(server_fd, 5) < 0) {
        close(server_fd);
        return;
    }

    while (server_running) {
        struct sockaddr_un client_addr;
        socklen_t len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len);
        if (client_fd >= 0) {
            HandleClient(client_fd);
        }
    }
}

void StartIpcServer() {
    if (server_running) return;
    server_running = true;
    server_thread = std::thread(ServerLoop);
    broadcaster_thread = std::thread(BroadcasterLoop);
    server_thread.detach();
    broadcaster_thread.detach();
}

void StopIpcServer() {
    server_running = false;
    queue_cv.notify_all();

    if (server_fd >= 0) {
        shutdown(server_fd, SHUT_RDWR);
        close(server_fd);
        server_fd = -1;
    }

    std::lock_guard<std::mutex> lock(clients_mutex);
    for (int fd : clients) {
        close(fd);
    }
    clients.clear();
}

void BroadcastData(const std::string& data) {
    if (!server_running) return;

    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        // Limit queue size to prevent memory explosion if broadcaster is slow
        if (message_queue.size() > 10) {
            message_queue.pop_front();
        }
        message_queue.push_back(data);
    }
    queue_cv.notify_one();
}
