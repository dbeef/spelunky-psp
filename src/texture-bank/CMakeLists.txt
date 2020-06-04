project(TextureBank)

add_library(TextureBank STATIC
        src/TextureBank.cpp
        src/TextureRegion.cpp
        interface/TextureBank.hpp
        interface/TextureType.hpp
        interface/TextureRegion.hpp
        interface/spritesheet-frames/HUDSpritesheetFrames.hpp
        interface/spritesheet-frames/MainMenuSpritesheetFrames.hpp
        interface/spritesheet-frames/MainDudeSpritesheetFrames.hpp
        )

target_include_directories(TextureBank
        PRIVATE include interface
        INTERFACE interface
        )

set_target_properties(TextureBank PROPERTIES
        CXX_STANDARD
        14
        )

target_link_libraries(TextureBank
    PUBLIC
        glad
        Renderer
    PRIVATE
        Logger
        GraphicsUtils
        cjson
        )

if (SPELUNKY_PSP_PLATFORM_LINUX)
    target_link_libraries(TextureBank PRIVATE dl)
    target_compile_definitions(TextureBank PRIVATE SPELUNKY_PSP_PLATFORM_LINUX)
elseif(SPELUNKY_PSP_PLATFORM_PSP)
    target_compile_definitions(TextureBank PRIVATE SPELUNKY_PSP_PLATFORM_PSP)
endif()
