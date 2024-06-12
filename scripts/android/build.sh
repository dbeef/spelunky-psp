#!/usr/bin/env bash

source variables.sh

# Following variables must be present in the environment:
#
# ANDROID_NDK_HOME - Root of directory with unpacked Android NDK, i.e - /opt/android/android-ndk-r15c
# ANDROID_DEPS_ARMEABI_V7A - Root of directory with armeabi-v7a dependency artifacts, i.e - /opt/android/deps-armeabi-v7a,
#                            will be forwarded to CMake through CMAKE_PREFIX_PATH.
# ANDROID_DEPS_ARM64_V8A - Root of directory with arm64-v8a dependency artifacts, i.e - /opt/android/deps-arm64-v8a,
#                          will be forwarded to CMake through CMAKE_PREFIX_PATH.

echo ANDROID_NDK_HOME: $ANDROID_NDK_HOME
echo ANDROID_DEPS_ARMEABI_V7A: $ANDROID_DEPS_ARMEABI_V7A
echo ANDROID_DEPS_ARM64_V8A: $ANDROID_DEPS_ARM64_V8A

# FIXME: This should be building under TMP_PATH, just like every other artifact:
cd $PLATFORMS_PATH/android
./gradlew assemble
cp build/android-app/outputs/apk/debug/Spelunky_PSP-debug.apk $INSTALL_PATH/Spelunky_PSP.apk
