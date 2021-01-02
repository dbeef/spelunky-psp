project(GraphicsUtils)

add_library(GraphicsUtils STATIC
        src/CreateTexture.cpp
        interface/graphics_utils/CreateTexture.hpp
        interface/graphics_utils/LookAt.hpp
        src/LookAt.cpp
)

target_include_directories(GraphicsUtils
        PRIVATE include interface
        INTERFACE interface
)

set_target_properties(GraphicsUtils PROPERTIES
        CXX_STANDARD
        17
)

target_link_libraries(GraphicsUtils
    PUBLIC
        glad
    PRIVATE
        stb
        Logger
        glm
        )
