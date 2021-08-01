project(Audio CXX ASM)

# TODO: Make a function for marking each file in passed directory
set_property(SOURCE generated/arrowtrap.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE generated/kiss.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE generated/title.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE generated/explosion.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE generated/pickup.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE generated/throw.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE generated/jetpack.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE generated/whip.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE generated/hurt.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE generated/coin.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE generated/chestopen.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE generated/die.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE generated/shotgun.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE generated/gem.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE generated/cave.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE generated/jump.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE generated/entering_door.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE generated/climb2.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE generated/hit.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE generated/bat.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE generated/climb1.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")

add_library(Audio STATIC
        include/AudioBank.hpp
        interface/audio/Audio.hpp
        interface/audio/SFXType.hpp
        interface/audio/MusicType.hpp
        # Compile AudioBank only when it is actually utilized on target platform:
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_PSP}>,src/AudioBank.cpp,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_WINDOWS}>,src/AudioBank.cpp,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_LINUX}>,src/AudioBank.cpp,>
        # Compile audio backend (dummy when audio is not supported):
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_PSP}>,src/Audio_SDL_mixer.cpp,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_WINDOWS}>,src/Audio_SDL_mixer.cpp,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_LINUX}>,src/Audio_SDL_mixer.cpp,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_ANDROID}>,src/Audio_Dummy.cpp,>
        generated/arrowtrap.wav.asm
        generated/kiss.wav.asm
        generated/title.wav.asm
        generated/explosion.wav.asm
        generated/pickup.wav.asm
        generated/throw.wav.asm
        generated/jetpack.wav.asm
        generated/whip.wav.asm
        generated/hurt.wav.asm
        generated/coin.wav.asm
        generated/chestopen.wav.asm
        generated/die.wav.asm
        generated/shotgun.wav.asm
        generated/gem.wav.asm
        generated/cave.wav.asm
        generated/jump.wav.asm
        generated/entering_door.wav.asm
        generated/climb2.wav.asm
        generated/hit.wav.asm
        generated/bat.wav.asm
        generated/climb1.wav.asm
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
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_PSP}>,SDL_1_XX,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_WINDOWS}>,SDL_1_XX,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_LINUX}>,SDL_1_XX,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_ANDROID}>,SDL_2_XX,>
)
