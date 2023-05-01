#!/usr/bin/env bash
# This script is meant to be run from the root of the project.

source scripts/building/variables-psp.sh

# Run, filtering all but internal, Spelunky-PSP output.
ppsspp $BUILD_PATH/EBOOT.PBP 2>&1 | grep -F SPELUNKY
