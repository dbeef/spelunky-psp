project(Renderer)

add_library(Renderer STATIC
        src/Renderer.cpp
        interface/Renderer.hpp
        interface/RenderEntity.hpp
        )

target_include_directories(Renderer
        PRIVATE interface
        INTERFACE interface
)

set_target_properties(Renderer PROPERTIES
        CXX_STANDARD
        14
)

target_link_libraries(Renderer
    PUBLIC
        TextureBank
        LevelGenerator
        RenderingUtils
    PRIVATE
        Logger
        Video
        GraphicsUtils
        cjson
        )

if (SPELUNKY_PSP_PLATFORM_LINUX)
    target_link_libraries(Renderer PRIVATE dl)
    target_compile_definitions(Renderer PRIVATE SPELUNKY_PSP_PLATFORM_LINUX)
elseif(SPELUNKY_PSP_PLATFORM_PSP)
    target_compile_definitions(Renderer PRIVATE SPELUNKY_PSP_PLATFORM_PSP)
endif()
