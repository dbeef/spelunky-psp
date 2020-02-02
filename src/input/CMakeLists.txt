project(Input)

add_library(Input STATIC
        src/Input.cpp
        interface/Input.hpp
        $<$<BOOL:SPELUNKY_PSP_PLATFORM_LINUX>:src/Input_Linux.cpp>
        $<$<BOOL:SPELUNKY_PSP_PLATFORM_PSP>:src/Input_PSP.cpp>
)

target_include_directories(Input
        PRIVATE include interface
        INTERFACE interface
)

set_target_properties(Input PROPERTIES
        CXX_STANDARD
        14
)

target_link_libraries(Input PRIVATE Logger Camera)
