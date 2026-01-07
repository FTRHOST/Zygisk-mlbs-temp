# Panduan Pengembangan

Dokumen ini ditujukan untuk pengembang yang ingin memodifikasi, mempelajari, atau menambahkan fitur baru ke kode sumber ini.

## 1. Struktur Kode & Alur Eksekusi

### Titik Masuk (Zygisk)
- **File:** `app/src/main/jni/main.cpp`
- **Kelas:** `MyModule` (mewarisi dari `zygisk::ModuleBase`)
- **Alur:**
  1. `onLoad`: Modul dimuat.
  2. `preAppSpecialize`: Mendeteksi jika proses yang berjalan adalah game target (`isGame`).
  3. `postAppSpecialize`: Jika game terdeteksi, thread baru `hack_prepare` diluncurkan (detached).

### Inisialisasi
- **File:** `app/src/main/jni/hack.cpp`
- **Fungsi `hack_prepare`:**
  1. Menunggu library game (`TargetLibName`) dimuat.
  2. Melakukan hook pada `eglSwapBuffers` (hanya untuk menggerakkan loop pembaruan thread utama).
  3. Memanggil `StartIpcServer` untuk memulai penyiar data.
  4. Memanggil `hack_start` untuk melampirkan IL2CPP dan memulai logika game.
- **Fungsi `hook_eglSwapBuffers`:**
  - Memanggil `MonitorBattleState()` untuk memperbarui dan menyiarkan data game.
  - TIDAK melakukan rendering apa pun.

## 2. Sistem IPC (`IpcServer.cpp`)
Menggantikan Web Server lama.
- **Mekanisme:** `AF_UNIX`, `SOCK_STREAM`.
- **Namespace:** Abstrak (dimulai dengan byte null `\0`).
- **Threading:**
  - `ServerLoop`: Menerima koneksi baru.
  - `BroadcasterLoop`: Membaca dari antrian pesan dan mengirim data ke semua klien.
  - **Non-blocking:** Menggunakan `O_NONBLOCK` dan antrian untuk memastikan thread game utama TIDAK PERNAH terblokir oleh I/O jaringan.

## 3. Logika Game & Pembacaan Memori (`GameLogic.cpp`)
- **Pembacaan Memori:** Menggunakan `Il2CppGetStaticFieldValue` dan aritmatika pointer manual untuk membaca `PlayerData`.
- **Serialisasi:** Konstruksi JSON manual menggunakan `std::stringstream` untuk menghindari library eksternal yang berat.
- **Penyiaran:** Memanggil `BroadcastData(json_string)` untuk mendorong pembaruan ke antrian IPC.

## 4. Keamanan & Hardening
- **Tanpa Hook Input:** Hook `InputConsumer` dihapus.
- **Tanpa Overlay:** ImGui dihapus.
- **Obfuscation:** String dibungkus dalam `OBFUSCATE("string")`. Pastikan Anda menyertakan `obfuscate.h` saat menambahkan string baru.

## 5. Menambahkan Fitur Baru
1. **Baca Memori:** Tambahkan logika di `GameLogic.cpp` untuk membaca data yang diinginkan.
2. **Serialisasi:** Tambahkan data baru ke pembuat string JSON di `MonitorBattleState`.
3. **Verifikasi:** Gunakan `check_mod.py` untuk melihat apakah kolom baru muncul di output.
