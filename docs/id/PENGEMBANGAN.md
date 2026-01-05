# Panduan Pengembangan (Developer Guide)

Dokumen ini ditujukan untuk pengembang yang ingin memodifikasi, mempelajari, atau menambahkan fitur baru ke dalam source code ini.

## 1. Struktur Kode & Alur Eksekusi

### Entry Point (Zygisk)
- **File:** `app/src/main/jni/main.cpp`
- **Kelas:** `MyModule` (turunan `zygisk::ModuleBase`)
- **Alur:**
  1. `onLoad`: Module dimuat.
  2. `preAppSpecialize`: Mendeteksi apakah proses yang berjalan adalah game target (`isGame`).
  3. `postAppSpecialize`: Jika game terdeteksi, thread baru `hack_prepare` diluncurkan (detached).

### Inisialisasi Cheat
- **File:** `app/src/main/jni/hack.cpp`
- **Fungsi `hack_prepare`:**
  1. Menunggu library game (`TargetLibName`) dimuat.
  2. Menginisialisasi hooking fungsi sistem (`dlsym` / `DobbyHook`) seperti `eglSwapBuffers` (untuk render ImGui) dan input events.
  3. Memanggil `hack_start` untuk memulai logika game.
- **Fungsi `hook_eglSwapBuffers`:**
  - Ini adalah *render loop*.
  - Menginisialisasi ImGui (`SetupImGui`) pada frame pertama.
  - Menggambar menu (`ImGui::NewFrame`, logic menu, `ImGui::Render`).
  - Memanggil `MonitorBattleState()` untuk update data game setiap frame.

## 2. Sistem Hooking
Hooking digunakan untuk membelokkan eksekusi kode game ke kode kita.
- **Library:** Menggunakan **Dobby** (atau implementasi internal di `utils.cpp`).
- **Implementasi:**
  ```cpp
  // Contoh di hack.cpp
  utils::hook((void*)target_func, (func_t)my_hook_func, (func_t*)&original_func);
  ```
- **Target Umum:**
  - `eglSwapBuffers`: Untuk menyuntikkan tampilan UI (Overlay).
  - `InputConsumer::initializeMotionEvent`: Untuk menangkap touch input agar menu bisa disentuh.

## 3. Penjelasan Fitur Khusus

### A. Web Server (`WebServer.cpp`)
Menggunakan library `cpp-httplib` untuk membuat server HTTP ringan.
- **Threading:** Server berjalan di thread terpisah (`std::thread server_thread`) agar tidak memblokir render loop game.
- **JSON Serialization:** Menggunakan `nlohmann/json` untuk mengubah struct C++ (`BattleStats`, `GlobalState`) menjadi string JSON.
- **Concurrency:** Karena data diakses dari thread game (render loop) dan thread server (request handler), `std::mutex` (`g_State.stateMutex`) digunakan untuk mencegah *data race*.

### B. Game Logic & Memory Reading (`GameLogic.cpp`)
Bagian ini bertanggung jawab membaca data dari memori game (IL2CPP).
- **IL2CPP Refelction:** Menggunakan fungsi helper `Il2CppGetStaticFieldValue` untuk mengakses instance singleton game (misal: `LogicBattleManager`, `ShowFightData`).
- **UpdatePlayerInfo:**
  - Membaca list pemain dari `SystemData_GetBattlePlayerInfo`.
  - Melakukan iterasi dan membaca field-field pada offset memori tertentu.
  - **PENTING:** Offset (seperti `SystemData_RoomData_...` di `ToString.h`/`GameClass.h`) harus diperbarui jika game mengalami update.
- **Battle Stats:**
  - Mengakses class `ShowFightDataTiny_Layout` untuk membaca kill, gold, exp.

## 4. Menambahkan Fitur Baru

### Menambah Checkbox di Menu
1. Buka `hack.cpp`.
2. Cari bagian `ImGui::Begin`.
3. Tambahkan `ImGui::Checkbox("Nama Fitur", &variable_boolean);`.
4. Implementasikan logika fitur tersebut (misalnya di dalam `hook_eglSwapBuffers` atau fungsi hook lainnya) menggunakan `if (variable_boolean) { ... }`.

### Menambah Endpoint API Baru
1. Buka `WebServer.cpp`.
2. Di dalam fungsi `RunServerLoop`, tambahkan:
   ```cpp
   svr->Get("/endpoint_baru", [](const httplib::Request &, httplib::Response &res) {
       // Logika anda
       res.set_content("Hello World", "text/plain");
   });
   ```

## 5. Tips Debugging
- Gunakan `LOGI`, `LOGE` (defined di `log.h`) untuk mencetak log ke Logcat.
- Filter Logcat di Android Studio dengan tag "Zygisk" atau nama library Anda.
- Jika game crash (Force Close), periksa offset memori. Offset yang salah adalah penyebab utama crash pada mod menu berbasis memori.
