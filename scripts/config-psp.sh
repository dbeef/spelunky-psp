#!/usr/bin/env bash
# This script is meant to be run from the root of the project.

# Cross compiles SpelunkyPSP for PSP, using following dependencies:
# * SDL 1.2 as a vendor of creating window, retrieving OpenGL context, controls and sounds.

INSTALL_PATH=`pwd`/_install-psp

rm -rf _build-psp
mkdir _build-psp
cd _build-psp

cmake .. \
-G "Unix Makefiles" \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INSTALL_PREFIX=$INSTALL_PATH \
-DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/PSP.cmake \
-DPSPDEV=/usr/local/pspdev

