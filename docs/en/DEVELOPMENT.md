# Development Guide

This document is intended for developers who want to modify, study, or add new features to this source code.

## 1. Code Structure & Execution Flow

### Entry Point (Zygisk)
- **File:** `app/src/main/jni/main.cpp`
- **Class:** `MyModule` (inherits from `zygisk::ModuleBase`)
- **Flow:**
  1. `onLoad`: Module is loaded.
  2. `preAppSpecialize`: Detects if the running process is the target game (`isGame`).
  3. `postAppSpecialize`: If the game is detected, a new thread `hack_prepare` is launched (detached).

### Cheat Initialization
- **File:** `app/src/main/jni/hack.cpp`
- **Function `hack_prepare`:**
  1. Waits for the game library (`TargetLibName`) to load.
  2. Initializes system function hooks (`dlsym` / `DobbyHook`) like `eglSwapBuffers` (for ImGui rendering) and input events.
  3. Calls `hack_start` to begin game logic.
- **Function `hook_eglSwapBuffers`:**
  - This is the *render loop*.
  - Initializes ImGui (`SetupImGui`) on the first frame.
  - Draws the menu (`ImGui::NewFrame`, menu logic, `ImGui::Render`).
  - Calls `MonitorBattleState()` to update game data every frame.

## 2. Hooking System
Hooking is used to detour game code execution to our code.
- **Library:** Uses **Dobby** (or internal implementation in `utils.cpp`).
- **Implementation:**
  ```cpp
  // Example in hack.cpp
  utils::hook((void*)target_func, (func_t)my_hook_func, (func_t*)&original_func);
  ```
- **Common Targets:**
  - `eglSwapBuffers`: To inject the UI (Overlay).
  - `InputConsumer::initializeMotionEvent`: To capture touch inputs so the menu can be interacted with.

## 3. Specific Feature Explanation

### A. Web Server (`WebServer.cpp`)
Uses the `cpp-httplib` library to create a lightweight HTTP server.
- **Threading:** Server runs on a separate thread (`std::thread server_thread`) to avoid blocking the game render loop.
- **JSON Serialization:** Uses `nlohmann/json` to convert C++ structs (`BattleStats`, `GlobalState`) into JSON strings.
- **Concurrency:** Since data is accessed from the game thread (render loop) and server thread (request handler), `std::mutex` (`g_State.stateMutex`) is used to prevent *data races*.

### B. Game Logic & Memory Reading (`GameLogic.cpp`)
This part is responsible for reading data from game memory (IL2CPP).
- **IL2CPP Reflection:** Uses helper functions `Il2CppGetStaticFieldValue` to access game singleton instances (e.g., `LogicBattleManager`, `ShowFightData`).
- **UpdatePlayerInfo:**
  - Reads player list from `SystemData_GetBattlePlayerInfo`.
  - Iterates and reads fields at specific memory offsets.
  - **IMPORTANT:** Offsets (like `SystemData_RoomData_...` in `ToString.h`/`GameClass.h`) must be updated if the game updates.
- **Battle Stats:**
  - Accesses `ShowFightDataTiny_Layout` class to read kills, gold, exp.

## 4. Adding New Features

### Adding a Checkbox to the Menu
1. Open `hack.cpp`.
2. Find the `ImGui::Begin` section.
3. Add `ImGui::Checkbox("Feature Name", &variable_boolean);`.
4. Implement the feature logic (e.g., inside `hook_eglSwapBuffers` or other hook functions) using `if (variable_boolean) { ... }`.

### Adding a New API Endpoint
1. Open `WebServer.cpp`.
2. Inside the `RunServerLoop` function, add:
   ```cpp
   svr->Get("/new_endpoint", [](const httplib::Request &, httplib::Response &res) {
       // Your logic
       res.set_content("Hello World", "text/plain");
   });
   ```

## 5. Debugging Tips
- Use `LOGI`, `LOGE` (defined in `log.h`) to print logs to Logcat.
- Filter Logcat in Android Studio with the tag "Zygisk" or your library name.
- If the game crashes (Force Close), check memory offsets. Incorrect offsets are the main cause of crashes in memory-based mod menus.
