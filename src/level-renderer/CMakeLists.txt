project(LevelRenderer)

add_library(LevelRenderer STATIC
        src/LevelRenderer.cpp
        interface/LevelRenderer.hpp
        interface/RenderTile.hpp
        interface/OAMEntry.hpp
        interface/OAMRegistry.hpp
        src/RenderTile.cpp
        src/OAMRegistry.cpp
)

target_include_directories(LevelRenderer
        PRIVATE include interface
        INTERFACE interface
)

set_target_properties(LevelRenderer PROPERTIES
        CXX_STANDARD
        14
)

target_link_libraries(LevelRenderer
        PRIVATE
            Logger
            stb
            Camera
            Video
            glm::glm
            GraphicsUtils
            TextureBank
        PUBLIC
            cjson
            glad
            LevelGenerator
        )

if (SPELUNKY_PSP_PLATFORM_LINUX)
    target_link_libraries(LevelRenderer PRIVATE dl)
    target_compile_definitions(LevelRenderer PRIVATE SPELUNKY_PSP_PLATFORM_LINUX)
elseif(SPELUNKY_PSP_PLATFORM_PSP)
    target_compile_definitions(LevelRenderer PRIVATE SPELUNKY_PSP_PLATFORM_PSP)
endif()
