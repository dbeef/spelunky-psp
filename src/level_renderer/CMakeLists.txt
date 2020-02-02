project(LevelRenderer)

add_library(LevelRenderer STATIC
        src/LevelRenderer.cpp
        interface/LevelRenderer.hpp
)

target_include_directories(LevelRenderer
        PRIVATE include interface
        INTERFACE interface
)

set_target_properties(LevelRenderer PROPERTIES
        CXX_STANDARD
        14
)

target_link_libraries(LevelRenderer PRIVATE Logger glad stb Camera LevelGenerator Video glm::glm GraphicsUtils)

if (SPELUNKY_PSP_PLATFORM_LINUX)
    target_link_libraries(LevelRenderer PRIVATE dl)
endif()
