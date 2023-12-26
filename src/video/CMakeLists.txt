project(Video)

add_library(Video STATIC
        interface/video/Video.hpp
        src/Video.cpp
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_PSP}>,src/Video_PSP.cpp,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_WINDOWS}>,src/Video_Desktop.cpp,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_LINUX}>,src/Video_Desktop.cpp,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_DARWIN}>,src/Video_Darwin.cpp,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_ANDROID}>,src/Video_Android.cpp,>
)

target_include_directories(Video
        PRIVATE include interface
        INTERFACE interface
)

set_target_properties(Video PROPERTIES
        CXX_STANDARD
        17
)

target_link_libraries(Video
    PUBLIC
        Time
        Viewport
    PRIVATE
        Logger
        glad
        Input
        GraphicsUtils
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_PSP}>,SDL_1_XX,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_WINDOWS}>,SDL_1_XX,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_LINUX}>,SDL_2_XX,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_DARWIN}>,SDL_2_XX,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_ANDROID}>,SDL_2_XX,>
        $<IF:$<BOOL:${SPELUNKY_PSP_WITH_IMGUI}>,imgui,>
        Dependencies
)
