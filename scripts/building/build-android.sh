#!/usr/bin/env bash
# This script is meant to be run from the root of the project.

source scripts/building/variables-android.sh

# Following variables must be present in the environment:
# ANDROID_NDK_HOME - Root of directory with unpacked Android NDK, i.e - /opt/android/android-ndk-r15c
# ANDROID_DEPS_ARMEABI_V7A - Root of directory with armeabi-v7a dependency artifacts, i.e - /opt/android/deps-armeabi-v7a,
#                            will be forwarded to CMake through CMAKE_PREFIX_PATH.
# ANDROID_DEPS_ARM64_V8A - Root of directory with arm64-v8a dependency artifacts, i.e - /opt/android/deps-arm64-v8a,
#                            will be forwarded to CMake through CMAKE_PREFIX_PATH.

rm -rf $INSTALL_PATH
mkdir -p $INSTALL_PATH

cd $ROOT_PATH/platforms/android
./gradlew assemble
cp build/android-app/outputs/apk/debug/Spelunky_PSP-debug.apk $INSTALL_PATH/Spelunky_PSP.apk
