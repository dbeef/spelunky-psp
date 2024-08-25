#!/bin/bash

source variables.sh

RESOURCE_COMPILER_PATH=${TOOLS_BUILD_PATH}/resource-compiler/resource-compiler
OUT_DIR_PATH="${ROOT_PATH}/src/assets/generated"
OUT_HEADER_DIR_PATH="${OUT_DIR_PATH}/files/"
INDEX_FILE_PATH="${OUT_DIR_PATH}/VirtualFileIndex.cpp"

cleanup() {
    rm -rf "${OUT_HEADER_DIR_PATH}"
    rm -rf "${INDEX_FILE_PATH}"
    mkdir -p ${OUT_HEADER_DIR_PATH}
}

resource_compile() {
    echo "Resource-compiling files"

    if [[ ! -f ${RESOURCE_COMPILER_PATH} ]]; then 
    {
        echo "Call tools.sh first"
        exit
    } fi
    
    cd ${ASSETS_PATH}/filesystem
    IN_FILE_PATHS=$(find . -type f | sed -e "s/\.\///g")
    for FILE_PATH in $IN_FILE_PATHS; do 
        FILENAME=$(basename $FILE_PATH)
        ${RESOURCE_COMPILER_PATH} ${FILE_PATH} $OUT_HEADER_DIR_PATH/$FILENAME.hpp
    done
    cd -
}

generate_virtual_file_index() {
    echo "Generating virtual file index at: ${INDEX_FILE_PATH}"

    touch ${INDEX_FILE_PATH}
    
    echo "// auto-generated at $(date +'%d.%m.%Y %H:%M')" >> ${INDEX_FILE_PATH}
    echo '#include "VirtualFileIndex.hpp"' >> ${INDEX_FILE_PATH}
    echo 'std::vector<VirtualFile> get_resource_compiled_files() {' >> ${INDEX_FILE_PATH}
    echo 'std::vector<VirtualFile> out;' >> ${INDEX_FILE_PATH}
    
    for FILE_PATH in $IN_FILE_PATHS; do 
        FILENAME=$(basename $FILE_PATH)
        echo "{" >> ${INDEX_FILE_PATH}
        echo "#include \"./files/${FILENAME}.hpp\"" >> ${INDEX_FILE_PATH}
        echo "out.push_back({path, &data[0], size});" >> ${INDEX_FILE_PATH}
        echo "}" >> ${INDEX_FILE_PATH}
    done
    
    echo 'return out;' >> ${INDEX_FILE_PATH}
    echo '}' >> ${INDEX_FILE_PATH}
}

cleanup
resource_compile
generate_virtual_file_index
