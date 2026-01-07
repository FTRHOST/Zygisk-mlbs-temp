# Introduction & Installation

This guide covers how to build, install, and verify the module.

## Prerequisites
- Android Device (Rooted with Magisk or KernelSU).
- Zygisk enabled in Magisk/KernelSU settings.
- **For Building:**
  - Android NDK (r27 recommended).
  - Java JDK 17+.
  - Git.
- **For Verification:**
  - Python 3 (installed on PC or Termux).
  - ADB (Android Debug Bridge).

## Installation
1. Download the `Zygisk-MLBS.zip` from the Releases page.
2. Open Magisk/KernelSU Manager.
3. Go to **Modules** -> **Install from Storage**.
4. Select the zip file.
5. Reboot your device.

## Usage
Since this is a stealth module, there is no visible menu.
1. Open the game.
2. The module runs automatically in the background.
3. To verify it is working and see the data:
   - **Method A (Logcat):**
     Run `adb logcat -s "Askan"` to see system logs.
   - **Method B (Data Viewer):**
     1. Push `check_mod.py` to your device (or run on PC with port forwarding setup).
     2. Run `python3 check_mod.py`.
     3. You should see real-time JSON data appearing in the console.

## Safety Notes
- Do not use this on your main account. Even with hardening, risk exists.
- The module creates no files on your internal storage (except standard system logs).
- Network traffic is fully local (IPC).
