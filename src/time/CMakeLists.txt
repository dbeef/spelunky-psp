project(Time)

add_library(Time STATIC
        interface/time/Timestep.hpp
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_PSP}>,src/Timestep_SDL2.cpp,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_WINDOWS}>,src/Timestep_SDL1.cpp,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_LINUX}>,src/Timestep_SDL1.cpp,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_DARWIN}>,src/Timestep_SDL2.cpp,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_ANDROID}>,src/Timestep_SDL2.cpp,>
)

target_include_directories(Time
        PRIVATE include interface
        INTERFACE interface
)

target_link_libraries(Time PUBLIC
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_PSP}>,SDL_2_XX,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_WINDOWS}>,SDL_1_XX,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_LINUX}>,SDL_2_XX,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_DARWIN}>,SDL_2_XX,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_ANDROID}>,SDL_2_XX,>
)
set_target_properties(Time PROPERTIES
        CXX_STANDARD
        17
)
