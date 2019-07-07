#!/usr/bin/env bash

# Builds SpelunkyPSP natively on Linux, using following dependencies:
# * SDL2 as a vendor of creating window, retrieving OpenGL context, controls and sounds.

INSTALL_PATH=`pwd`/_install-linux

rm -rf _build-linux
mkdir _build-linux
cd _build-linux

cmake .. \
-G "Unix Makefiles" \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INSTALL_PREFIX=$INSTALL_PATH

cmake --build . --target install --config Release
