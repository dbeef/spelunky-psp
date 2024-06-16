#!/bin/bash

source variables.sh

cd $ROOT_PATH
sudo docker run \
    --rm \
    -it \
    -v $(pwd):/root \
    pspdev/pspdev:latest \
    /bin/bash -c "cd /root/scripts/psp/ && ./config.sh && ./build.sh"
sudo chown -R $(whoami) $TMP_PATH
