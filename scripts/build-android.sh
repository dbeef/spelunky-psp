#!/usr/bin/env bash
# This script is meant to be run from the root of the project.

source scripts/variables-android.sh

cmake --build $BUILD_PATH --target install --config Release -j `nproc`
