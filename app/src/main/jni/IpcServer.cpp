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

void HandleClient(int client_fd) {
    SetNonBlocking(client_fd);
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.push_back(client_fd);
    }
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
                    // Real error, disconnect
                    close(fd);
                    it = clients.erase(it);
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
    const char* name = "mlbs_ipc"; // Plain text here, obfuscate in usage if needed, but socket name is local
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
