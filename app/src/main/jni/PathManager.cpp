#include "PathManager.h"
#include <fstream>
#include <unistd.h>
#include <android/log.h>

// Helper function to get the current process's base package name
static std::string getPackageName() {
    char buffer[1024] = {0};
    FILE* cmdline = fopen("/proc/self/cmdline", "r");
    if (cmdline) {
        fread(buffer, sizeof(buffer), 1, cmdline);
        fclose(cmdline);
        
        std::string full_cmd(buffer);
        // The process name can be "com.package.name:SomeProcessName"
        // We only want the base package name.
        size_t colon_pos = full_cmd.find(':');
        if (colon_pos != std::string::npos) {
            return full_cmd.substr(0, colon_pos);
        }
        // If no colon, return the full string (for standard processes)
        if (!full_cmd.empty()) {
            return full_cmd;
        }
    }
    __android_log_print(ANDROID_LOG_ERROR, "MLBS_PATH", "Failed to get package name from /proc/self/cmdline");
    return ""; // Return empty on failure
}

std::string getDynamicFilesDir() {
    static std::string files_dir_cache; // Cache the result so we don't read procfs every time
    if (!files_dir_cache.empty()) {
        return files_dir_cache;
    }

    std::string pkgName = getPackageName();
    if (!pkgName.empty()) {
        // Use the accessible external storage path as requested
        files_dir_cache = "/storage/emulated/0/Android/data/" + pkgName + "/files";
        return files_dir_cache;
    }
    return ""; // Return empty on failure
}


std::string getDynamicConfigPath() {
    std::string filesDir = getDynamicFilesDir();
    if (!filesDir.empty()) {
        return filesDir + "/config.json";
    }
    return ""; // Return empty on failure
}
