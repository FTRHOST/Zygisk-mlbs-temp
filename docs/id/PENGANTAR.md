# Pengantar & Instalasi

Panduan ini mencakup cara membangun, menginstal, dan memverifikasi modul.

## Prasyarat
- Perangkat Android (Rooted dengan Magisk atau KernelSU).
- Zygisk diaktifkan di pengaturan Magisk/KernelSU.
- **Untuk Membangun (Build):**
  - Android NDK (r27 direkomendasikan).
  - Java JDK 17+.
  - Git.
- **Untuk Verifikasi:**
  - Python 3 (diinstal di PC atau Termux).
  - ADB (Android Debug Bridge).

## Instalasi
1. Unduh `Zygisk-MLBS.zip` dari halaman Rilis (Releases).
2. Buka Magisk/KernelSU Manager.
3. Pergi ke **Modules** -> **Install from Storage**.
4. Pilih file zip tersebut.
5. Reboot perangkat Anda.

## Penggunaan
Karena ini adalah modul stealth (tersembunyi), tidak ada menu yang terlihat.
1. Buka game.
2. Modul berjalan secara otomatis di latar belakang.
3. Untuk memverifikasi apakah berfungsi dan melihat data:
   - **Metode A (Logcat):**
     Jalankan `adb logcat -s "Askan"` untuk melihat log sistem.
   - **Metode B (Penampil Data):**
     1. Masukkan `check_mod.py` ke perangkat Anda (atau jalankan di PC dengan pengaturan port forwarding).
     2. Jalankan `python3 check_mod.py`.
     3. Anda akan melihat data JSON muncul secara real-time di konsol.

## Catatan Keamanan
- Jangan gunakan ini di akun utama Anda. Meskipun sudah diperkeras (hardened), risiko tetap ada.
- Modul tidak membuat file di penyimpanan internal Anda (kecuali log sistem standar).
- Lalu lintas jaringan sepenuhnya lokal (IPC).
