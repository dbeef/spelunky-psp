#!/bin/sh
# Filter all but internal, Spelunky-PSP output.
ppsspp _build-psp/SpelunkyPSP.PBP 2>&1 | grep -F SPELUNKY
