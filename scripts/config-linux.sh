#!/usr/bin/env bash
# This script is meant to be run from the root of the project.

# Builds SpelunkyPSP natively on Linux, using following dependencies:
# * SDL2 as a vendor of creating window, retrieving OpenGL context, controls and sounds.

ROOT_PATH=`pwd`
INSTALL_PATH=$ROOT_PATH/tmp/install-linux

rm -rf tmp/build-linux
mkdir -p tmp/build-linux

(cd tmp/build-linux && cmake ../../ \
-G "Unix Makefiles" \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INSTALL_PREFIX=$INSTALL_PATH \
-DCMAKE_MODULE_PATH=$ROOT_PATH/vendor/sdl2-cmake-modules \
-DCMAKE_FIND_DEBUG_MODE=ON)
