#!/usr/bin/env bash
# This script is meant to be run from the root of the project.

# Cross compiles SpelunkyPSP for PSP, using following dependencies:
# * SDL 1.2 as a vendor of creating window, retrieving OpenGL context, controls and sounds.

INSTALL_PATH=`pwd`/_install-psp

cd _build-psp
cmake --build . --target install --config Release

