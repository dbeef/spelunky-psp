macro(add_linux_dependencies)
    add_library(Dependencies INTERFACE)
    target_link_libraries(Dependencies INTERFACE
            -lSDL
            -lGLEW
            -lGL
            -lX11
            -ldl
            )

    target_compile_definitions(Dependencies INTERFACE
            SPELUNKY_PSP_PLATFORM_LINUX
            )
endmacro()

macro(spelunky_psp_post_build_linux)
endmacro()
