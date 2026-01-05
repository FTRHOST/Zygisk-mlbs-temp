#pragma once

#include <string>

// Fungsi ini akan mendapatkan path absolut dan dinamis ke file konfigurasi.
// Contoh: "/data/data/com.mobile.legends/files/config.json"
std::string getDynamicConfigPath();

// Fungsi ini akan mendapatkan path absolut ke direktori 'files'
// Contoh: "/data/data/com.mobile.legends/files"
std::string getDynamicFilesDir();
