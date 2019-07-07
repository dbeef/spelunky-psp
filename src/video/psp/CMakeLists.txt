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

set_target_properties(Video
        PROPERTIES
        CXX_STANDARD
        14
        COMPILE_FLAGS
        # Treat every warning as an error
        -Werror
        )
