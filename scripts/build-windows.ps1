# This script is meant to be run from the root of the project.

# Builds SpelunkyPSP natively on Windows, using following dependencies:
# * SDL as a vendor of creating window, retrieving OpenGL context, controls and sounds.

$ErrorActionPreference = "Stop"

$PROC_COUNT = (Get-WmiObject -class Win32_ComputerSystem).numberoflogicalprocessors

cmake --build "tmp/build-windows" --target install --config Release -j $PROC_COUNT
