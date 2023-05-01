#!/usr/bin/env bash
# This script is meant to be run from the root of the project.

# Builds SpelunkyPSP natively on Linux, using following dependencies:
# * SDL2 as a vendor of creating window, retrieving OpenGL context, controls and sounds.

cmake --build tmp/build-linux --target install --config Release -j `nproc`
