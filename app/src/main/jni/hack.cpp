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

#include "struct/Vector3.hpp"
#include "struct/Vector2.h"
#include "struct/Color.h"
#include "struct/Rect.h"
#include "struct/Quaternion.h"
#include "struct/MonoString.h"

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
    
    // 1. Loop ini menahan proses sampai 'liblogic.so' benar-benar muncul
    // Proses :ping tidak punya liblogic.so, jadi dia akan terjebak looping di sini selamanya.
    // Server IPC TIDAK AKAN PERNAH NYALA di proses :ping.
    do {
        sleep(1);
        g_TargetModule = utils::find_module(TargetLibName);
    } while (g_TargetModule.size <= 0);

    LOGI("%s: %p - %p", TargetLibName, g_TargetModule.start_address, g_TargetModule.end_address);

    // 2. [PERBAIKAN] Nyalakan Server IPC DI SINI.
    // Karena kita sudah melewati loop di atas, kita YAKIN 100% ini adalah PROSES UTAMA.
    LOGI("MLBS_CORE: Main Process confirmed (LibLogic found). Starting IPC Server...");
    g_State.roomInfoEnabled = true;
    StartIpcServer(); 

    // 3. Lanjut Attach
    Il2CppAttach(TargetLibName);
    g_IsGameReady = true;
}

void hack_prepare(const char *_game_data_dir) {
    LOGI("hack thread: %d", gettid());
    int api_level = utils::get_android_api_level();
    LOGI("api level: %d", api_level);

    void *egl_handle = xdl_open(OBFUSCATE("libEGL.so"), 0);
    void *eglSwapBuffers = xdl_sym(egl_handle, OBFUSCATE("eglSwapBuffers"), nullptr);
    if (NULL != eglSwapBuffers) {
        utils::hook((void*)eglSwapBuffers, (func_t)hook_eglSwapBuffers, (func_t*)&old_eglSwapBuffers);
    }
    xdl_close(egl_handle);

    // Default enable features since we have no menu
    g_State.roomInfoEnabled = true;

    hack_start(_game_data_dir);
}
