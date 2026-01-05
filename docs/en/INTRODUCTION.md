# Introduction to Android Mod Menu Template

This repository is a mod menu template for Android games (specifically IL2CPP based) using ImGui for the user interface. It is designed to work with Zygisk (Magisk Module) for code injection into the game process.

## Requirements
- **Rooted Android** with Magisk or KernelSU.
- **Zygisk** enabled in Magisk/KernelSU settings.
- Basic knowledge of C++ and Android NDK.

## How to Use
1. **Clone Repository:**
   ```bash
   git clone <repository_url>
   ```
2. **Configure Target Game:**
   - Open `app/src/main/jni/game.h` (if exists) or `main.cpp`.
   - Change `pkgName` or the package detection logic to match your target game.
   - Update `TargetLibName` in `hack.cpp` to the main game library name (e.g., `libil2cpp.so` or `libUE4.so`).

3. **Build:**
   - Open the project in Android Studio.
   - Build APK (Debug or Release).
   - Install the generated APK on your Android device.

4. **Enable Module:**
   - This APK acts as an installer/host for the Zygisk module.
   - After installation, reboot or follow specific Zygisk instructions. *Note: This template uses the Zygisk module approach; ensure you understand how to load Zygisk modules.*

## Main Folder Structure
- `app/src/main/jni/`: Contains all C++ source code.
  - `imgui/`: ImGui library.
  - `feature/`: Game-specific feature logic.
  - `hack.cpp`: Entry point for menu initialization and hooks.
  - `main.cpp`: Zygisk module entry point.
  - `GameLogic.cpp`: Game data reading logic (Battle Stats, Player Info).
  - `WebServer.cpp`: Local HTTP server to expose game data.

For further details, please read:
- [Features & Usage (User Guide)](FEATURES.md)
- [Development Guide](DEVELOPMENT.md)
