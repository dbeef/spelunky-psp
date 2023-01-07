#!/usr/bin/env bash
# This script is meant to be run from the root of the project.

cmake \
    --build tmp/build-emscripten \
    --target install \
    --config Release \
    -j `nproc`
