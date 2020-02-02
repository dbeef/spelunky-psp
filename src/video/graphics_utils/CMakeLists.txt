project(GraphicsUtils)

add_library(GraphicsUtils STATIC
        src/CreateTexture.cpp
        interface/graphics_utils/CreateTexture.hpp
        )

target_include_directories(GraphicsUtils
        PRIVATE include interface
        INTERFACE interface
        )

set_target_properties(GraphicsUtils PROPERTIES
        CXX_STANDARD
        14
        )

target_link_libraries(GraphicsUtils PRIVATE glad stb Logger)
