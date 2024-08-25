#!/bin/bash

source variables.sh

# Capped at maximum texture size that can be created on PSP;
# Note that emulators may not represent this limitation, 
# manifesting the problem only on a real device:
SPRITESHEET_WIDTH=512
SPRITESHEET_HEIGHT=512

INPUT_DIR_PATHS=$(find ${ASSETS_PATH}/development/tilesheets/* -type d)
ATLASC_PATH=${TOOLS_BUILD_PATH}/atlasc/atlasc
OUTPUT_DIRECTORY=${ASSETS_PATH}/filesystem/tilesheets/

if [[ ! -f ${ATLASC_PATH} ]]; then 
{
    echo "Call tools.sh first"
    exit
} fi

mkdir -p ${OUTPUT_DIRECTORY}

for IN_DIR in ${INPUT_DIR_PATHS}; do 
    cd ${IN_DIR}
    OUTPUT_FILENAME=$(basename ${IN_DIR})
    OUTPUT_FILE_PATH=${OUTPUT_DIRECTORY}/${OUTPUT_FILENAME}
    ${ATLASC_PATH} \
        *.png \
        -o ${OUTPUT_DIRECTORY}/${OUTPUT_FILENAME} \
        -B 0 \
        -P 0 \
        -m \
        -W ${SPRITESHEET_WIDTH} \
        -H ${SPRITESHEET_HEIGHT} \
        -2 \
        -A 0
    # Add missing metadata extension:
    mv ${OUTPUT_FILE_PATH} ${OUTPUT_FILE_PATH}.json
    echo "Size: $(du -sh ${OUTPUT_FILE_PATH}.png)"
    cd -
done

echo "Success!"

