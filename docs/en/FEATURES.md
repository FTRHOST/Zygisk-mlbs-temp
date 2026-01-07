# Feature Documentation

This document explains the features available in this mod, focusing on the safety and data broadcasting capabilities.

## 1. Stealth Architecture
Unlike traditional mod menus, this module is designed to be invisible.
- **No Overlay:** There is no on-screen menu or button. This avoids "Overlay Detection" and "Input Injection Detection".
- **No Network Port:** The module does not open any TCP/UDP ports (like HTTP servers). This avoids "Port Scanning" detection.
- **Background Operation:** The module runs silently attached to the game process, reading memory and sending data only when available.

## 2. IPC Broadcasting (Unix Domain Socket)
Instead of a Web Server, this mod uses **Inter-Process Communication (IPC)** via a Unix Domain Socket (Abstract Namespace) to share data.

- **Socket Name:** `\0mlbs_ipc` (Abstract namespace, no file on disk).
- **Access:** You need a specialized client to read this data.
- **Verification:** You can use the provided `check_mod.py` script (requires Python and adb/termux) to connect and see the data stream.

### Data Format
The module broadcasts JSON data separated by newlines.
Example Data:
```json
{
  "state": 2,
  "players": [
    {
      "name": "PlayerName",
      "rank": "Mythic",
      "hero": "Layla",
      "camp": 1
    }
  ]
}
```

## 3. Room Info & Player Info
This feature reads game memory to obtain detailed information about other players in the match or lobby.

- **Data Provided:** Name, Rank, Hero, Spell, Rank Level, etc.
- **Usage:** Run `check_mod.py` or your custom external overlay tool to see this data in real-time.

## 4. Battle Stats
Monitors game statistics in real-time.
- **Team A vs Team B:**
  - Kills, Gold, EXP, Towers, Objectives.
- **Note:** This data is serialized into the IPC stream when valid.

## 5. String Obfuscation
All sensitive strings (library names, function names) are encrypted at compile-time using XOR obfuscation to prevent static analysis strings detection.
