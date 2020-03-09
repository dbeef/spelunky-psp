project(Video)

add_library(Video STATIC
        interface/video/Context.hpp
        src/Context.cpp
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_PSP}>,src/Context_PSP.cpp,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_LINUX}>,src/Context_Linux.cpp,>
        src/Context.cpp)

target_include_directories(Video
        PRIVATE include interface
        INTERFACE interface
)

set_target_properties(Video PROPERTIES
        CXX_STANDARD
        14
)

target_link_libraries(Video PRIVATE Logger Camera glad Input GraphicsUtils PUBLIC Time)
