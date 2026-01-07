# Android Mod Template (Zygisk + IPC) (MLBS)

![GitHub](https://img.shields.io/github/license/LGLTeam/Android-Mod-Menu?style=flat-square)

Stealthy Zygisk module template for Android games (IL2CPP) focused on safety and anti-cheat evasion. This repository provides a base for creating game mods that broadcast data via **IPC (Unix Domain Sockets)** instead of using visual overlays or network ports.

## Documentation / Dokumentasi

Complete documentation is available in the `docs/` folder.
Dokumentasi lengkap tersedia di dalam folder `docs/`.

### 🇮🇩 Bahasa Indonesia
- **[Pengantar & Instalasi](docs/id/PENGANTAR.md)**: Mulai di sini. Cara build dan install.
- **[Fitur & Penggunaan](docs/id/FITUR.md)**: Penjelasan fitur Room Info, IPC Broadcasting, dll.
- **[Panduan Pengembangan](docs/id/PENGEMBANGAN.md)**: Penjelasan kode (hooks, IPC, logic) untuk developer.

### 🇺🇸 English
- **[Introduction & Installation](docs/en/INTRODUCTION.md)**: Start here. How to build and install.
- **[Features & Usage](docs/en/FEATURES.md)**: Explanation of Room Info, IPC Broadcasting, etc.
- **[Development Guide](docs/en/DEVELOPMENT.md)**: Code explanation (hooks, IPC, logic) for developers.

---

## Features Overview
- **Stealth Mode:** No visual overlay, no open network ports, no input hooks.
- **Zygisk Module:** Clean injection method using Magisk/KernelSU.
- **IPC Broadcasting:** Broadcasts game data (Room Info, Stats) via Unix Domain Sockets (Abstract Namespace) for external tools to read.
- **Memory Hacking:** Example implementation of reading game memory (IL2CPP) safely.
- **String Obfuscation:** Compile-time encryption for sensitive strings.

## Credits
- **LGLTeam**
- **MJx0 (KittyMemory)**
- **Rprop (And64InlineHook)**

## License
GNU General Public License 3
