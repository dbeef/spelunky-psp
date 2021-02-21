# This script is meant to be run from the root of the project.

# Builds SpelunkyPSP natively on Windows, using following dependencies:
# * SDL as a vendor of creating window, retrieving OpenGL context, controls and sounds.

$ErrorActionPreference = "Stop"

cmake --build "tmp/build-windows" --target install --config Release -j $env:NUMBER_OF_PROCESSORS
