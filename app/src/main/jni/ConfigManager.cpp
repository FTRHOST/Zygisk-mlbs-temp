#include "ConfigManager.h"
#include "GlobalState.h"
#include "PathManager.h" // Include the new dynamic path manager
#include <fstream>
#include <android/log.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include "include/nlohmann/json.hpp"

// Forward declaration for internal save function
static void _SaveConfig_Internal(GlobalState& state);

void LoadConfig(GlobalState& state) {
    std::string configPath = getDynamicConfigPath();
    if (configPath.empty()) {
        __android_log_print(ANDROID_LOG_ERROR, "MLBS_CONFIG", "Failed to get dynamic config path for loading.");
        return;
    }

    std::lock_guard<std::mutex> lock(state.stateMutex);
    std::ifstream configFile(configPath);
    if (!configFile.is_open()) {
        __android_log_print(ANDROID_LOG_INFO, "MLBS_CONFIG", "Config file not found at %s. Generating new config...", configPath.c_str());
        _SaveConfig_Internal(state);
        return;
    }

    nlohmann::json j;
    try {
        configFile >> j;
        if (j.contains("roomInfoEnabled") && j["roomInfoEnabled"].is_boolean()) {
            state.roomInfoEnabled = j["roomInfoEnabled"];
        }
        if (j.contains("webServerEnabled") && j["webServerEnabled"].is_boolean()) {
            state.webServerEnabled = j["webServerEnabled"];
        }
        if (j.contains("showMenu") && j["showMenu"].is_boolean()) {
            state.showMenu = j["showMenu"];
        }
        __android_log_print(ANDROID_LOG_INFO, "MLBS_CONFIG", "Successfully loaded config from %s", configPath.c_str());
    } catch (const nlohmann::json::parse_error& e) {
        __android_log_print(ANDROID_LOG_ERROR, "MLBS_CONFIG", "Failed to parse config file: %s", e.what());
    }
}

// Internal function to save config without locking mutex
static void _SaveConfig_Internal(GlobalState& state) {
    std::string filesDir = getDynamicFilesDir();
    if (filesDir.empty()) {
        __android_log_print(ANDROID_LOG_ERROR, "MLBS_CONFIG", "Failed to get dynamic files directory for saving.");
        return;
    }
    
    // The OS should create the parent /data/data/pkg.name/ directory. We just ensure 'files' exists.
    __android_log_print(ANDROID_LOG_INFO, "MLBS_CONFIG", "Ensuring directory exists: %s", filesDir.c_str());
    if (mkdir(filesDir.c_str(), 0777) != 0 && errno != EEXIST) {
        __android_log_print(ANDROID_LOG_ERROR, "MLBS_CONFIG", "Failed to create directory %s. Error: %s", filesDir.c_str(), strerror(errno));
        // We continue anyway, maybe the file can still be opened
    }

    std::string configPath = getDynamicConfigPath();
    if (configPath.empty()) {
         __android_log_print(ANDROID_LOG_ERROR, "MLBS_CONFIG", "Failed to get dynamic config path for saving.");
        return;
    }

    nlohmann::json j;
    j["roomInfoEnabled"] = state.roomInfoEnabled;
    j["webServerEnabled"] = state.webServerEnabled;
    j["showMenu"] = state.showMenu;

    std::ofstream configFile(configPath);
    if (!configFile.is_open()) {
        __android_log_print(ANDROID_LOG_ERROR, "MLBS_CONFIG", "Failed to open config file for writing: %s. Error: %s", configPath.c_str(), strerror(errno));
        return;
    }

    configFile << j.dump(4);
    __android_log_print(ANDROID_LOG_INFO, "MLBS_CONFIG", "Successfully saved config to %s", configPath.c_str());
}

void SaveConfig(GlobalState& state) {
    std::lock_guard<std::mutex> lock(state.stateMutex);
    _SaveConfig_Internal(state);
}
