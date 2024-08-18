$ErrorActionPreference = "Stop"

$ROOT_PATH = "$(Get-Location)/../../"
$TMP_PATH = "$ROOT_PATH/tmp"
$DEPS_PATH = "$TMP_PATH/deps-windows/SDL"
$INSTALL_PATH = "$TMP_PATH/install-windows"
$BUILD_PATH = "$TMP_PATH/build-windows"

New-Item -Path "$BUILD_PATH" -ItemType "Directory"

cmake `
    -S $ROOT_PATH `
    -B "$BUILD_PATH" `
    -DSPELUNKY_PSP_USE_VIRTUAL_FILESYSTEM=ON `
    -DCMAKE_INSTALL_PREFIX="$INSTALL_PATH" `
    -DCMAKE_PREFIX_PATH="$DEPS_PATH"
