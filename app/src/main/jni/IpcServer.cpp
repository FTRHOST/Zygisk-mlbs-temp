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
#include "GameLogic.h"

#define TAG "MLBS_IPC"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

static int server_fd = -1;
static std::atomic<bool> server_running(false);
static std::vector<int> clients;
static std::mutex clients_mutex;

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
    LOGI("Received command: %s", cmd.c_str());

    std::lock_guard<std::mutex> lock(g_State.stateMutex);

    // Legacy Commands (Mapped to RoomInfo or Master Toggle?)
    // User requested "Setiap Fitur" (Every feature).

    // Command format: SET_CONFIG <FeatureName> <1/0>
    // Example: SET_CONFIG RoomInfo 0

    if (cmd.find("SET_CONFIG") != std::string::npos) {
        std::stringstream ss(cmd);
        std::string command, feature;
        int value;

        ss >> command >> feature >> value;

        bool enabled = (value != 0);
        bool changed = false;

        if (feature == "RoomInfo") {
            if (g_State.config.RoomInfo != enabled) {
                g_State.config.RoomInfo = enabled;
                changed = true;
            }
        } else if (feature == "BattleStats") {
            if (g_State.config.BattleStats != enabled) {
                g_State.config.BattleStats = enabled;
                changed = true;
            }
        } else if (feature == "BattleTimer") {
            if (g_State.config.BattleTimer != enabled) {
                g_State.config.BattleTimer = enabled;
                changed = true;
            }
        } else if (feature == "LogicPlayerStats") {
             if (g_State.config.LogicPlayerStats != enabled) {
                g_State.config.LogicPlayerStats = enabled;
                changed = true;
            }
        }

        if (changed) {
            SaveConfig(g_State.config);
            BroadcastData("{\"type\":\"log\", \"msg\":\"Config Updated: " + feature + "=" + (enabled ? "ON" : "OFF") + "\"}");
        }
    }
    // Keep legacy start/stop for overall disable?
    // User said "disable dan enable using IPC".
    // I'll assume legacy start/stop toggles RoomInfo (main feature) or maybe all?
    // Let's make them toggle RoomInfo for backward compatibility if needed,
    // but the new system is SET_CONFIG.
}

void ClientReaderLoop(int client_fd) {
    LOGI("Reader loop started for FD: %d", client_fd);
    char buffer[1024];
    while (server_running) {
        ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            std::string cmd(buffer);
            HandleIncomingCommand(cmd);
        } else if (bytes_read == 0) {
            LOGI("Client FD %d disconnected (EOF)", client_fd);
            break;
        } else {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            } else {
                LOGE("Client FD %d read error: %s", client_fd, strerror(errno));
                break;
            }
        }
    }

    close(client_fd);
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        auto it = std::find(clients.begin(), clients.end(), client_fd);
        if (it != clients.end()) {
            clients.erase(it);
            LOGI("Client FD %d removed from list", client_fd);
        }
    }
}

void HandleClient(int client_fd) {
    SetNonBlocking(client_fd);
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.push_back(client_fd);
    }
    LOGI("New client accepted! FD: %d. Total clients: %zu", client_fd, clients.size());
    std::thread(ClientReaderLoop, client_fd).detach();
}

void BroadcasterLoop() {
    LOGI("Broadcaster Thread Started");
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
            if (clients.empty()) {
                // LOGI("Broadcasting message but no clients connected.");
            }
            for (auto it = clients.begin(); it != clients.end(); ) {
                int fd = *it;
                std::string msg = message + "\n";
                ssize_t sent = send(fd, msg.c_str(), msg.length(), MSG_NOSIGNAL);

                if (sent < 0) {
                    if (errno != EAGAIN && errno != EWOULDBLOCK) {
                        LOGE("Send failed to FD %d: %s", fd, strerror(errno));
                        // Let Reader Loop handle cleanup? Or force it here?
                        // If we force cleanup here, we must be careful with Reader Loop.
                        // Ideally, a broken pipe error means we should close.
                        // For debugging, let's just log.
                    }
                } else {
                    // LOGI("Sent %zd bytes to FD %d", sent, fd);
                }
                ++it;
            }
        }
    }
    LOGI("Broadcaster Thread Stopped");
}

void ServerLoop() {
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd < 0) {
        LOGE("Failed to create socket: %s", strerror(errno));
        return;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;

    const char* obf_name = OBFUSCATE("mlbs_ipc");
    // Ensure abstract namespace by explicitly handling the null byte logic
    // Name in sun_path: \0mlbs_ipc
    addr.sun_path[0] = '\0';
    strncpy(addr.sun_path + 1, obf_name, sizeof(addr.sun_path) - 2);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(sa_family_t) + 1 + strlen(obf_name)) < 0) {
        LOGE("Bind failed: %s", strerror(errno));
        close(server_fd);
        return;
    }

    if (listen(server_fd, 5) < 0) {
        LOGE("Listen failed: %s", strerror(errno));
        close(server_fd);
        return;
    }

    LOGI("IPC Server Listening on \\0%s", obf_name);

    while (server_running) {
        struct sockaddr_un client_addr;
        socklen_t len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len);
        if (client_fd >= 0) {
            HandleClient(client_fd);
        } else {
            // LOGE("Accept failed: %s", strerror(errno));
            // Sleep to avoid busy loop if accept fails repeatedly
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
    LOGI("IpcServer Started");
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
        if (message_queue.size() > 10) {
            message_queue.pop_front();
        }
        message_queue.push_back(data);
    }
    queue_cv.notify_one();
}
