add_library(stb STATIC
        src/stb_image.cpp
        )

target_include_directories(stb
        INTERFACE
        include
        PRIVATE
        include
        )

set_target_properties (stb PROPERTIES
    FOLDER vendor
)
