project(Video)

add_library(Video STATIC
        interface/video/Video.hpp
        src/Video.cpp
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_PSP}>,src/Video_PSP.cpp,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_DESKTOP}>,src/Video_Desktop.cpp,>
        )

target_include_directories(Video
        PRIVATE include interface
        INTERFACE interface
)

set_target_properties(Video PROPERTIES
        CXX_STANDARD
        14
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
        SDL_1_XX
        )
