# Dokumentasi Fitur

Dokumen ini menjelaskan fitur-fitur yang tersedia dalam mod menu ini, baik fitur standar maupun fitur lanjutan seperti Web Server dan Room Info.

## 1. Floating Mod Menu (ImGui)
Fitur utama adalah menu melayang (floating menu) yang digambar di atas game menggunakan library **ImGui**.
- **Akses:** Menu dapat diperkecil/diperbesar (toggle) biasanya dengan menyentuh icon atau tombol tertentu (tergantung konfigurasi input).
- **Fungsi:** Berisi checkbox dan tombol untuk mengaktifkan cheat/fitur.
- **Tampilan:**
  - **Menu "Zygisk MLBS (White)":** Window utama.
  - **Features:** Bagian untuk mengaktifkan Web Server, Room Info, dan tombol Save Config.
  - **Player Info:** Bagian collapsible (bisa dilipat) yang menampilkan daftar pemain dalam room saat ini.

## 2. Web Server (HTTP API)
Fitur unik ini menjalankan server HTTP lokal di perangkat Android saat game berjalan. Ini memungkinkan perangkat lain (atau browser di HP yang sama) untuk melihat data game secara real-time.

- **Cara Mengaktifkan:** Centang "Web Server" di menu.
- **Status:** Teks "Running" (Hijau) atau "Stopped" (Merah) di menu.
- **Port:** Server berjalan di port `2626`.
- **Akses:** Buka browser dan ketik `http://<IP_HP_ANDA>:2626/<endpoint>`.

### Endpoint API
Berikut adalah endpoint yang tersedia untuk mengambil data (JSON format):

| Endpoint | Deskripsi | Data yang Dikembalikan |
| :--- | :--- | :--- |
| `/state` | Status ringkas game. | State battle, status fitur, list pemain (basic). |
| `/inforoom` | Info detail Room/Lobby. | Data lengkap setiap pemain (ID, Rank, Hero, Skin, dll). |
| `/infobattle` | Statistik pertarungan real-time. | Kill, Gold, EXP, Tower Kills, Lord/Turtle Kills untuk kedua tim. |
| `/timebattle` | Durasi pertandingan. | Waktu berjalan dalam detik. |
| `/config` | Konfigurasi (POST). | Mengubah setting menu via HTTP request. |

## 3. Room Info & Player Info
Fitur ini membaca memori game untuk mendapatkan informasi detail tentang pemain lain di dalam pertandingan atau lobby.

- **Tampilan di Menu:** Tabel berisi Camp (Tim), Nama, dan Rank.
- **Data Detail (via Web Server):**
  - **Basic:** Nama, UID, Rank, Hero, Spell.
  - **Advanced:** Win rate (implied), Hero Power, MMR, Skin yang dipakai, Negara asal, dll.
  - **Manfaat:** Mengetahui kekuatan musuh sebelum pertandingan dimulai (jika data tersedia di fase draft pick).

## 4. Battle Stats (Statistik Pertarungan)
Memonitor statistik game secara real-time yang mungkin tidak selalu terlihat di UI standar game.
- **Tim A vs Tim B:**
  - Total Kill.
  - Total Gold & EXP.
  - Jumlah Tower hancur.
  - Objektif (Lord/Turtle/LingZhu/ShenGui) yang dibunuh.
- **Kegunaan:** Analisis performa tim secara mendalam atau untuk kebutuhan casting/streaming overlay via Web Server.

## 5. Save Config
- **Fungsi:** Menyimpan pengaturan menu (posisi window, cheat yang aktif) ke penyimpanan internal.
- **Lokasi File:** `/data/data/<package_game>/files/imgui.ini` dan config JSON internal.
- **Auto-Load:** Pengaturan akan dimuat otomatis saat game dijalankan kembali.
