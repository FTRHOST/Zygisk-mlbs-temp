# Dokumentasi Fitur

Dokumen ini menjelaskan fitur yang tersedia di mod ini, dengan fokus pada keamanan dan penyiaran data (broadcasting).

## 1. Arsitektur Stealth (Tersembunyi)
Berbeda dengan mod menu tradisional, modul ini dirancang agar tidak terlihat.
- **Tanpa Overlay:** Tidak ada menu atau tombol di layar. Ini menghindari "Deteksi Overlay" dan "Deteksi Input Injection".
- **Tanpa Port Jaringan:** Modul tidak membuka port TCP/UDP (seperti web server). Ini menghindari deteksi "Port Scanning".
- **Operasi Latar Belakang:** Modul berjalan diam-diam menempel pada proses game, membaca memori dan mengirim data hanya jika tersedia.

## 2. IPC Broadcasting (Unix Domain Socket)
Sebagai pengganti Web Server, mod ini menggunakan **Inter-Process Communication (IPC)** melalui Unix Domain Socket (Abstract Namespace) untuk berbagi data.

- **Nama Socket:** `\0mlbs_ipc` (Abstract namespace, tidak ada file di disk).
- **Akses:** Anda memerlukan klien khusus untuk membaca data ini.
- **Verifikasi:** Anda dapat menggunakan skrip `check_mod.py` (membutuhkan Python dan adb/termux) untuk terhubung dan melihat aliran data.

### Format Data
Modul menyiarkan data JSON yang dipisahkan oleh baris baru (newline).
Contoh Data:
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

## 3. Info Room & Info Pemain
Fitur ini membaca memori game untuk mendapatkan informasi rinci tentang pemain lain dalam pertandingan atau lobi.

- **Data yang Disediakan:** Nama, Rank, Hero, Spell, Level Rank, dll.
- **Penggunaan:** Jalankan `check_mod.py` atau alat overlay eksternal kustom Anda untuk melihat data ini secara real-time.

## 4. Statistik Pertempuran (Battle Stats)
Memantau statistik permainan secara real-time.
- **Tim A vs Tim B:**
  - Kill, Gold, EXP, Turret, Objektif.
- **Catatan:** Data ini diserialisasi ke dalam aliran IPC ketika valid.

## 5. Pengaburan String (Obfuscation)
Semua string sensitif (nama library, nama fungsi) dienkripsi saat compile-time menggunakan XOR obfuscation untuk mencegah deteksi string analisis statis.
