project(Audio CXX ASM)

# TODO: Make a function for marking each file in passed directory
set_property(SOURCE include/generated/arrowtrap.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE include/generated/kiss.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE include/generated/title.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE include/generated/explosion.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE include/generated/pickup.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE include/generated/throw.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE include/generated/jetpack.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE include/generated/whip.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE include/generated/hurt.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE include/generated/coin.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE include/generated/chestopen.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE include/generated/doit.py.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE include/generated/die.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE include/generated/shotgun.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE include/generated/gem.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE include/generated/cave.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE include/generated/jump.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE include/generated/entering_door.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE include/generated/climb2.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE include/generated/hit.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE include/generated/bat.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")
set_property(SOURCE include/generated/climb1.wav.asm APPEND PROPERTY COMPILE_OPTIONS "-x"  "assembler-with-cpp")

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
        include/generated/arrowtrap.wav.asm
        include/generated/kiss.wav.asm
        include/generated/title.wav.asm
        include/generated/explosion.wav.asm
        include/generated/pickup.wav.asm
        include/generated/throw.wav.asm
        include/generated/jetpack.wav.asm
        include/generated/whip.wav.asm
        include/generated/hurt.wav.asm
        include/generated/coin.wav.asm
        include/generated/chestopen.wav.asm
        include/generated/doit.py.asm
        include/generated/die.wav.asm
        include/generated/shotgun.wav.asm
        include/generated/gem.wav.asm
        include/generated/cave.wav.asm
        include/generated/jump.wav.asm
        include/generated/entering_door.wav.asm
        include/generated/climb2.wav.asm
        include/generated/hit.wav.asm
        include/generated/bat.wav.asm
        include/generated/climb1.wav.asm
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
