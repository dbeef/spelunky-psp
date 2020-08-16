project(Audio)

add_library(Audio STATIC
        include/AudioBank.hpp
        interface/audio/Audio.hpp
        interface/audio/SFXType.hpp
        interface/audio/MusicType.hpp
        src/Audio.cpp
        src/AudioBank.cpp
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
        SDL_1_XX
        )
