$ErrorActionPreference = "Stop"

cmake --build "../../tmp/build-windows" --target install --config Release -j $env:NUMBER_OF_PROCESSORS
