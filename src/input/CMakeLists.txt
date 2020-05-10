project(Input)

add_library(Input STATIC
        src/Input.cpp
        interface/Input.hpp
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_PSP}>,src/Input_PSP.cpp,>
        $<IF:$<BOOL:${SPELUNKY_PSP_PLATFORM_DESKTOP}>,src/Input_Desktop.cpp,>
)

target_include_directories(Input
        PRIVATE include interface
        INTERFACE interface
)

set_target_properties(Input PROPERTIES
        CXX_STANDARD
        14
)

target_link_libraries(Input PRIVATE Logger Camera Dependencies)
