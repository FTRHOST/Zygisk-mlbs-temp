#include "GlobalState.h"
#include "PathManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <android/log.h>

#define TAG "MLBS_CONFIG"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

// Simple helper to parse boolean from JSON-like string
bool ParseBool(const std::string& content, const std::string& key, bool defaultValue) {
    std::string keyPattern = "\"" + key + "\":";
    size_t pos = content.find(keyPattern);
    if (pos == std::string::npos) return defaultValue;

    size_t valuePos = pos + keyPattern.length();
    // Skip whitespace
    while (valuePos < content.length() && (content[valuePos] == ' ' || content[valuePos] == '\t')) {
        valuePos++;
    }

    if (content.substr(valuePos, 4) == "true") return true;
    if (content.substr(valuePos, 5) == "false") return false;

    // Check for 1 or 0
    if (content[valuePos] == '1') return true;
    if (content[valuePos] == '0') return false;

    return defaultValue;
}

void LoadConfig(Config& config) {
    std::string path = getDynamicConfigPath();
    if (path.empty()) {
        LOGI("Config path is empty, using defaults");
        return;
    }

    std::ifstream file(path);
    if (!file.is_open()) {
        LOGI("Config file not found at %s, creating default", path.c_str());
        SaveConfig(config);
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    config.RoomInfo = ParseBool(content, "RoomInfo", true);
    config.BattleStats = ParseBool(content, "BattleStats", true);
    config.BattleTimer = ParseBool(content, "BattleTimer", true);
    config.LogicPlayerStats = ParseBool(content, "LogicPlayerStats", true);

    LOGI("Config Loaded: RoomInfo=%d, BattleStats=%d, BattleTimer=%d",
         config.RoomInfo, config.BattleStats, config.BattleTimer);
}

void SaveConfig(const Config& config) {
    std::string path = getDynamicConfigPath();
    if (path.empty()) return;

    std::ofstream file(path);
    if (file.is_open()) {
        file << "{\n";
        file << "  \"RoomInfo\": " << (config.RoomInfo ? "true" : "false") << ",\n";
        file << "  \"BattleStats\": " << (config.BattleStats ? "true" : "false") << ",\n";
        file << "  \"BattleTimer\": " << (config.BattleTimer ? "true" : "false") << ",\n";
        file << "  \"LogicPlayerStats\": " << (config.LogicPlayerStats ? "true" : "false") << "\n";
        file << "}\n";
        LOGI("Config Saved to %s", path.c_str());
    } else {
        LOGI("Failed to save config to %s", path.c_str());
    }
}
