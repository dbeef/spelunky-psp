project(Audio)

add_library(Audio STATIC
        interface/audio/Audio.hpp
        interface/audio/SFXType.hpp
        interface/audio/MusicType.hpp
        src/Audio_SDL_mixer.cpp
)

if (NOT SPELUNKY_PSP_PLATFORM_WINDOWS)
    set_target_properties(Audio PROPERTIES
            COMPILE_FLAGS
            # FIXME: Suppressing silly warning from resource-compiled assets.
            #        Changing outputted type to std::int8_t should fix it.
            "-Wno-error=narrowing -Wno-everything"
    )
endif()

target_include_directories(Audio
        PRIVATE include interface
        INTERFACE interface
)

set_target_properties(Audio PROPERTIES
        CXX_STANDARD
        17
)

target_link_libraries(Audio
        PUBLIC
        Patterns
        PRIVATE
        Logger
        Assets
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_PSP}>,SDL_2_XX,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_WINDOWS}>,SDL_1_XX,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_LINUX}>,SDL_2_XX,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_DARWIN}>,SDL_2_XX,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_ANDROID}>,SDL_2_XX,>
)
