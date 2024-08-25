#!/usr/bin/env bash

# Stop on error:
set -e
# Referencing unset variables are errors:
set -u

NUM_THREADS=$(nproc)
ROOT_PATH=$(realpath ../../)
ASSETS_PATH=$ROOT_PATH/assets
TMP_PATH=$ROOT_PATH/tmp

SPELUNKY_PSP_INSTALL_PATH=$TMP_PATH/install-linux-spelunky-psp
SPELUNKY_PSP_BUILD_PATH=$TMP_PATH/build-linux-spelunky-psp

TOOLS_INSTALL_PATH=$TMP_PATH/install-linux-tools
TOOLS_BUILD_PATH=$TMP_PATH/build-linux-tools
TOOLS_PATH=$ROOT_PATH/tools
