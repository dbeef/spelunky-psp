add_library(Video STATIC
        src/video/Context.cpp
        interface/video/Context.hpp
        interface/video/GL.hpp
        )

target_include_directories(Video
        PUBLIC
        interface
        PRIVATE
        interface/video
        )

target_link_libraries(Video PUBLIC glad Input)

set_target_properties(Video
        PROPERTIES
        CXX_STANDARD
        14
        CXX_EXTENSIONS
        OFF
        COMPILE_FLAGS
        # Treat every warning as an error
        -Werror
        )
