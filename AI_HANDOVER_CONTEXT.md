# AI HANDOVER CONTEXT: MLBS Zygisk Module

**Target Audience:** Software Engineering AI / LLM
**Role:** Senior Technical Lead
**Date:** 2026-01-11
**Severity:** CRITICAL

---

## 1. Project Overview
This project is a **Zygisk Module for Android** targeting the game *Mobile Legends: Bang Bang (MLBB)*.
**Core Objective:** Extract real-time game data (Battle Stats, Player Info, Rank, Cooldowns) directly from the game's memory (Il2Cpp/Unity) and broadcast it via a local UNIX Domain Socket (`\0mlbs_ipc`) to a companion receiver (e.g., Python Relay).

**Current State:**
- **Stable:** Crashing issues via direct pointer dereference have been resolved using `utils::read_address`.
- **Feature Set:** "BanPick" features have been removed. Focus is purely on Data Scraping.
- **Control:** Implemented hot-reloadable config (`config.json`) to toggle scraping ON/OFF.

---

## 2. Tech Stack & Dependencies

| Component | Technology | Version / Note |
| :--- | :--- | :--- |
| **Language** | C++ (C++17 Standard) | Native JNI Development |
| **Build System** | Shell Script + `ndk-build` | `package.sh` calls `ndk-build` manually |
| **Injection** | Zygisk (Magisk) | Uses `zygisk.hpp` API |
| **Memory** | KittyMemory | Included in `app/src/main/jni/include/KittyMemory` |
| **Hooking** | Dobby | Prebuilt libs in `app/src/main/jni/dobby` |
| **IPC** | Abstract UNIX Socket | Raw socket implementation in `IpcServer.cpp` |
| **Serialization** | Manual JSON (std::stringstream) | **Warning:** Error-prone. See Refactoring Permission. |

**Dependency Policy:**
> You are **AUTHORIZED** to recommend or implement updates to libraries (e.g., replacing manual JSON construction with `nlohmann/json`) if it improves stability and type safety, provided the `Android.mk` build configuration is updated accordingly.

---

## 3. Project Structure

- **`app/src/main/jni/`**: Root native source.
    - **`GameLogic.cpp`**: **CORE LOGIC**. Handles scanning, data extraction, JSON formatting, and state monitoring.
    - **`GameLogic.h`**: Headers for logic.
    - **`GlobalState.h`**: Thread-safe global state definitions (`g_State`).
    - **`IpcServer.cpp`**: Handles the socket connection to the external relay.
    - **`main.cpp`**: Entry point for Zygisk injection.
    - **`utils.h/cpp`**: Memory reading helpers. **CRITICAL**.
- **`app/src/main/jni/feature/GameClass.h`**: Il2Cpp Offset definitions and Macro mappings. **High Volatility** (Changes with Game Updates).
- **`app/src/main/jni/struct/LogicPlayer.h`**: Struct definitions matching internal Unity classes.
- **`package.sh`**: The build and packaging orchestrator.

---

## 4. Coding Standards (CRITICAL)

### Naming Conventions
- **Classes/Structs:** `PascalCase` (e.g., `LogicPlayerStats`).
- **Variables:** `camelCase` (e.g., `currentBattleState`).
- **Global Variables:** Prefix `g_` + `PascalCase` (e.g., `g_State`).
- **Macros/Offsets:** `UPPER_SNAKE_CASE` (e.g., `READ_PTR`, `LogicPlayer_m_ID`).

### Architecture: Polling & State Machine
- The module operates on a `frameTick` loop (hooked or threaded).
- **Update Frequency:**
    - `Heartbeat`: 1Hz (Every 60 ticks).
    - `BattleStats`: ~4Hz (Every 15 ticks) - Only when `battleState >= 3`.
    - `PlayerInfo`: ~1Hz (Every 60 ticks).

### Clean Code Rules
1.  **Memory Safety (Rule #0):**
    - **NEVER** dereference a pointer directly (e.g., `int x = *ptr`).
    - **ALWAYS** use `utils::read_address((void*)ptr, &target, sizeof(target))`.
    - **REASON:** The game memory is volatile. Direct access causes immediate SEGFAULTs.
2.  **String Safety:**
    - Use the provided `SafeReadString(uintptr_t)` helper. Never try to read `MonoString*` raw characters.
3.  **Concurrency:**
    - Access to `g_State` MUST be guarded by `std::lock_guard<std::mutex> lock(g_State.stateMutex)`.
4.  **Early Return:**
    - Verify pointers (e.g., `if (!logicBattleManager) return;`) immediately. Do not nest deep logic.

---

## 5. Critical Business Logic

### `MonitorBattleState()` in `GameLogic.cpp`
This function is the brain. It controls *when* data is read.
- **Constraint:** Do NOT read `LogicPlayer` data if `currentBattleState < 3` (Loading/Lobby). It leads to race conditions with the game's initialization.
- **Config Check:** It checks `config.json` every 180 ticks. If disabled, it clears vectors and halts processing.

### `SafeReadString()` implementation
Custom implementation to read Unity `MonoString` (Length at `0x10`, UTF-16 chars at `0x14`).
- **Validation:** Must check if length > 0 and length < 1024 before reading buffer.

---

## 6. Known Issues & Common Mistakes (READ BEFORE EDITING)

1.  **JSON Truncation/Syntax Errors:**
    - *Context:* We are manually building JSON strings using `std::stringstream`.
    - *Mistake:* Forgetting to escape quotes (`\"`) or leaving trailing commas in loops.
    - *Solution:* Be extremely meticulous with `ss <<` construction.
2.  **Tool Output Limits:**
    - *Context:* The AI `write_file` tool has token limits.
    - *Mistake:* Trying to write the entire `GameLogic.cpp` (~800 lines) in one go often results in truncation at the end.
    - *Solution:* When editing `GameLogic.cpp`, use `replace` for small changes, or split writes into chunks (e.g., write headers, then write logic) if rewriting the file.
3.  **Offset Invalidity:**
    - If data returns 0 or garbage, `GameClass.h` or `LogicPlayer.h` offsets likely point to the wrong memory address due to a game update.

---

## 7. Refactoring Permission

**You are GRANTED permission to:**
1.  **Implement a JSON Library:** Replacing the manual `std::stringstream` JSON construction with a lightweight, header-only JSON library (like `nlohmann/json` or `rapidjson`) is **Highly Recommended** to prevent syntax errors.
2.  **Refactor `GameLogic.cpp`:** Splitting this monolithic file into `GameLogic_Scanner.cpp` and `GameLogic_Broadcaster.cpp` is permitted to improve maintainability.
3.  **Optimize Macros:** Converting the `READ_FIELD` macros to C++ templates is encouraged for better type safety.

**Condition:** Changes must maintain the `utils::read_address` safety mechanism. Refactoring must not bypass memory safety checks.

---
**End of Context**