#!/bin/bash
set -e

# Build the release version
./gradlew assembleRelease

# Create the module directory
rm -rf build/zygisk_module
mkdir -p build/zygisk_module/zygisk
mkdir -p build/zygisk_module/META-INF/com/google/android

# Copy module.prop
cp app/src/main/assets/module.prop build/zygisk_module/

# Copy and rename native libraries
for abi in app/build/intermediates/stripped_native_libs/release/out/lib/*; do
    abi_name=$(basename "$abi")
    cp "$abi/libzygisk_mlbs.so" "build/zygisk_module/zygisk/$abi_name.so"
done

# Create installer scripts
echo "#!/sbin/sh
################# # Initialization #################
umask 022
# echo before loading util_functions
ui_print() { echo \"\$1\"; }
require_new_magisk() {
  ui_print \"*******************************\"
  ui_print \" Please install Magisk v20.4+! \"
  ui_print \"*******************************\"
  exit 1
}
######################### # Load util_functions.sh #########################
OUTFD=\$2
ZIPFILE=\$3
mount /data 2>/dev/null
[ -f /data/adb/magisk/util_functions.sh ] || require_new_magisk
. /data/adb/magisk/util_functions.sh
[ \$MAGISK_VER_CODE -lt 20400 ] && require_new_magisk
install_module
exit 0
" > build/zygisk_module/META-INF/com/google/android/update-binary
echo "#MAGISK" > build/zygisk_module/META-INF/com/google/android/updater-script

# Zip the module
(cd build/zygisk_module && zip -r ../Zygisk-ImGui-Menu.zip .)
echo "Zygisk module created at build/Zygisk-ImGui-Menu.zip"
