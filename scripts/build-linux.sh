#!/usr/bin/env bash
# This script is meant to be run from the root of the project.

# Builds SpelunkyPSP natively on Linux, using following dependencies:
# * SDL2 as a vendor of creating window, retrieving OpenGL context, controls and sounds.

cd tmp/build-linux
cmake --build . --target install --config Release -- -j 4
