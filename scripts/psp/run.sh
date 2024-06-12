#!/usr/bin/env bash

source variables.sh

# Run filtering everything except Spelunky-PSP output:
ppsspp $BUILD_PATH/EBOOT.PBP 2>&1 | grep -F SPELUNKY
