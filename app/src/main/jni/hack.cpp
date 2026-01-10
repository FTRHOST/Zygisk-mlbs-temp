#include <cstdint>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <dlfcn.h>
#include <string>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include "obfuscate.h"
#include "hack.h"
#include "log.h"
#include "game.h"
#include "utils.h"
#include "xDL/xdl.h"
#include "fake_dlfcn.h"
#include "Il2Cpp.h"
#include "GameLogic.h"
#include "IpcServer.h"
#include "KittyMemory/MemoryPatch.h"

static bool                 g_IsGameReady = false;
static utils::module_info   g_TargetModule{};

EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    if (g_IsGameReady) {
        MonitorBattleState();
    }
    return old_eglSwapBuffers(dpy, surface);
}

void hack_start(const char *_game_data_dir) {
    LOGI("hack start | %s", _game_data_dir);
    
    // 1. Wait for library
    do {
        sleep(1);
        g_TargetModule = utils::find_module(TargetLibName);
    } while (g_TargetModule.size <= 0);

    LOGI("%s: %p - %p", TargetLibName, g_TargetModule.start_address, g_TargetModule.end_address);

    // 2. Start IPC
    LOGI("MLBS_CORE: Main Process confirmed (LibLogic found). Starting IPC Server...");
    StartIpcServer(); 

    // 3. Attach Il2Cpp and Init Logic
    Il2CppAttach(TargetLibName);
    InitGameLogic();

    g_IsGameReady = true;
}

void hack_prepare(const char *_game_data_dir) {
    LOGI("hack thread: %d", gettid());

    void *egl_handle = xdl_open(OBFUSCATE("libEGL.so"), 0);
    void *eglSwapBuffers = xdl_sym(egl_handle, OBFUSCATE("eglSwapBuffers"), nullptr);
    if (NULL != eglSwapBuffers) {
        utils::hook((void*)eglSwapBuffers, (func_t)hook_eglSwapBuffers, (func_t*)&old_eglSwapBuffers);
    }
    xdl_close(egl_handle);

    // Default enable features
    g_State.roomInfoEnabled = true;

    hack_start(_game_data_dir);
}
