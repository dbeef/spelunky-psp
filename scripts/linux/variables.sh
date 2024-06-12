#!/usr/bin/env bash

# Stop on error:
set -e
# Referencing unset variables are errors:
set -u

ROOT_PATH=$(realpath ../../)
TMP_PATH=$ROOT_PATH/tmp
INSTALL_PATH=$TMP_PATH/install-linux
BUILD_PATH=$TMP_PATH/build-linux
NUM_THREADS=$(nproc)
