project(Audio)

add_library(Audio STATIC
        include/AudioBank.hpp
        interface/audio/Audio.hpp
        interface/audio/SFXType.hpp
        interface/audio/MusicType.hpp
        src/AudioBank.cpp
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_PSP}>,src/Audio_SDL_mixer.cpp,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_WINDOWS}>,src/Audio_SDL_mixer.cpp,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_LINUX}>,src/Audio_SDL_mixer.cpp,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_ANDROID}>,src/Audio_Dummy.cpp,>
        )

target_include_directories(Audio
        PRIVATE include interface
        INTERFACE interface
        )

set_target_properties(Audio PROPERTIES
        CXX_STANDARD
        14
        )

target_link_libraries(Audio
        PUBLIC
        Patterns
        PRIVATE
        Logger
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_PSP}>,SDL_1_XX,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_WINDOWS}>,SDL_1_XX,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_LINUX}>,SDL_1_XX,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_ANDROID}>,SDL_2_XX,>
)
