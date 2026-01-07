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

### Initialization
- **File:** `app/src/main/jni/hack.cpp`
- **Function `hack_prepare`:**
  1. Waits for the game library (`TargetLibName`) to load.
  2. Hooks `eglSwapBuffers` (only to drive the main thread update loop).
  3. Calls `StartIpcServer` to begin the data broadcaster.
  4. Calls `hack_start` to attach IL2CPP and begin game logic.
- **Function `hook_eglSwapBuffers`:**
  - Calls `MonitorBattleState()` to update and broadcast game data.
  - Does NOT perform any rendering.

## 2. IPC System (`IpcServer.cpp`)
Replaces the old Web Server.
- **Mechanism:** `AF_UNIX`, `SOCK_STREAM`.
- **Namespace:** Abstract (starts with null byte `\0`).
- **Threading:**
  - `ServerLoop`: Accepts new connections.
  - `BroadcasterLoop`: Reads from a message queue and sends data to all clients.
  - **Non-blocking:** Uses `O_NONBLOCK` and a queue to ensure the main game thread is NEVER blocked by network I/O.

## 3. Game Logic & Memory Reading (`GameLogic.cpp`)
- **Memory Reading:** Uses `Il2CppGetStaticFieldValue` and manual pointer arithmetic to read `PlayerData`.
- **Serialization:** Manual JSON construction using `std::stringstream` to avoid heavy external libraries.
- **Broadcasting:** Calls `BroadcastData(json_string)` to push updates to the IPC queue.

## 4. Security & Hardening
- **No Input Hooks:** `InputConsumer` hooks were removed.
- **No Overlay:** ImGui was removed.
- **Obfuscation:** Strings are wrapped in `OBFUSCATE("string")`. Ensure you include `obfuscate.h` when adding new strings.

## 5. Adding New Features
1. **Read Memory:** Add logic in `GameLogic.cpp` to read the desired data.
2. **Serialize:** Append the new data to the JSON string builder in `MonitorBattleState`.
3. **Verify:** Use `check_mod.py` to see if the new field appears in the output.
