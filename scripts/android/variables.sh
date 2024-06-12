#!/usr/bin/env bash

# Stop on error:
set -e
# Referencing unset variables are errors:
set -u

ROOT_PATH=$(realpath ../../)
TMP_PATH=$ROOT_PATH/tmp
INSTALL_PATH=$TMP_PATH/install-android
BUILD_PATH=$TMP_PATH/build-android
# TODO: Think about moving this directory somewhere else or more specific naming;
#       "platform-specific/android", "platform-files/android", "extras/android"?
PLATFORMS_PATH=$ROOT_PATH/platforms
