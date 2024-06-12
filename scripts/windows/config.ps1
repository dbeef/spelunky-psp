[CmdletBinding()]
param (
    [Parameter()][String]
    $DepsDirectory="deps"
)

$ErrorActionPreference = "Stop"

$ROOT = $(Get-Location)/../../
$INSTALL_PATH = "$ROOT/tmp/install-windows"
$DEPS_PATH = "$ROOT/$DepsDirectory/SDL"

if (Test-Path "$ROOT/tmp/build-windows")
{
    Remove-Item -Force -Recurse "$ROOT/tmp/build-windows"
}
New-Item -Path "$ROOT/tmp/build-windows" -ItemType "Directory"

cmake $ROOT `
    -B"$ROOT/tmp/build-windows" `
    -DCMAKE_INSTALL_PREFIX="$INSTALL_PATH" `
    -DCMAKE_PREFIX_PATH="$DEPS_PATH"
