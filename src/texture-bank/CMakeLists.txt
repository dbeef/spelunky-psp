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
        interface/spritesheet-frames/FontSpritesheetFrames.hpp
        )

target_include_directories(TextureBank
        PRIVATE include interface
        INTERFACE interface
        )

set_target_properties(TextureBank PROPERTIES
        CXX_STANDARD
        14
        )

if (NOT SPELUNKY_PSP_PLATFORM_WINDOWS)
set_target_properties(TextureBank PROPERTIES
        COMPILE_FLAGS
        # FIXME: Suppressing silly warning from resource-compiled assets.
        #        Changing outputted type to std::int8_t should fix it.
        "-Wno-error=narrowing"
        )
endif()

target_link_libraries(TextureBank
    PUBLIC
        Renderer
        Patterns
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
