# Pengantar Android Mod Menu Template

Repository ini adalah template mod menu untuk game Android (khususnya yang berbasis IL2CPP) menggunakan ImGui untuk antarmuka pengguna (UI). Template ini dirancang untuk bekerja dengan Zygisk (Magisk Module) untuk injeksi kode ke dalam proses game.

## Persyaratan
- **Android Rooted** dengan Magisk atau KernelSU.
- **Zygisk** diaktifkan di pengaturan Magisk/KernelSU.
- Pengetahuan dasar tentang C++ dan Android NDK.

## Cara Menggunakan
1. **Clone Repository:**
   ```bash
   git clone <repository_url>
   ```
2. **Konfigurasi Game Target:**
   - Buka `app/src/main/jni/game.h` (jika ada) atau `main.cpp`.
   - Ubah `pkgName` atau logika deteksi paket sesuai dengan game target Anda.
   - Ubah `TargetLibName` di `hack.cpp` ke nama library game utama (misalnya `libil2cpp.so` atau `libUE4.so`).

3. **Build:**
   - Buka project di Android Studio.
   - Build APK (Debug atau Release).
   - Install APK yang dihasilkan ke perangkat Android Anda.

4. **Aktifkan Modul:**
   - APK ini sebenarnya bertindak sebagai installer/host untuk modul Zygisk.
   - Setelah install, mungkin perlu reboot atau ikuti instruksi spesifik Zygisk (biasanya modul akan muncul di daftar modul Magisk jika dikemas dengan benar, atau aplikasi ini menginjeksikan dirinya sendiri). *Catatan: Template ini menggunakan pendekatan Zygisk module, pastikan Anda memahami cara load module Zygisk.*

## Struktur Folder Utama
- `app/src/main/jni/`: Berisi semua kode source C++.
  - `imgui/`: Library ImGui.
  - `feature/`: Logika fitur spesifik game.
  - `hack.cpp`: Entry point untuk inisialisasi menu dan hooks.
  - `main.cpp`: Entry point Zygisk module.
  - `GameLogic.cpp`: Logika pembacaan data game (Battle Stats, Player Info).
  - `WebServer.cpp`: Server HTTP lokal untuk mengekspos data game.

Untuk detail lebih lanjut, silakan baca:
- [Fitur & Penggunaan (User Guide)](FITUR.md)
- [Panduan Pengembangan (Developer Guide)](PENGEMBANGAN.md)
