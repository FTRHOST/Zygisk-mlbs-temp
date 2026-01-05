# Introduction to Android Mod Menu Templates

This repository is a mod menu template for Android games (specifically those based on IL2CPP) using ImGui for the user interface (UI). This template is designed to work with Zygisk (Magisk Module) for code injection into the game process.

## Requirements
- **Rooted Android** with Magisk or KernelSU.
- **Zygisk** enabled in Magisk/KernelSU settings.
- Basic knowledge of C++ and the Android NDK.

## How to Use
1. **Clone Repository:**
```bash
git clone https://github.com/FTRHOST/Zygisk-mlbs-temp.git
```
2. **Configure Target Game:**
- Open `app/src/main/jni/game.h` (if present) or `main.cpp`.
- Change `pkgName` or package detection logic according to your target game.
- Change `TargetLibName` in `hack.cpp` to the name of the main game library (e.g., `libil2cpp.so` or `libUE4.so`).

3. **Build:**
- Open the project in Android Studio or the NDK.
- Build the module by running the ./package.sh script.
- Install the resulting module to your Android device.

4. **Activate Module:**
- Install the build for the Zygisk module.
- After installation, you may need to reboot or follow Zygisk-specific instructions (usually the module will appear in the Magisk module list if packaged correctly). *Note: This template uses the Zygisk module approach; make sure you understand how to load Zygisk modules.*

## Main Folder Structure
- `app/src/main/jni/`: Contains all C++ source code.
- `imgui/`: The ImGui library.
- `feature/`: Game-specific feature logic.
- `hack.cpp`: Entry point for menu initialization and hooks.
- `main.cpp`: Entry point for the Zygisk module.
- `GameLogic.cpp`: Logic for reading game data (Battle Stats, Player Info).
- `WebServer.cpp`: Local HTTP server for exposing game data.

For more details, please read:
- [Features & Usage (User Guide)](FEATURES.md)
- [Developer Guide)](DEVELOPMENT.md)
