macro(add_miyoo_dependencies)
    add_library(SDL_1_XX INTERFACE)
    target_link_libraries(SDL_1_XX INTERFACE
        -lSDL
        )

    add_library(Dependencies INTERFACE)
    target_link_libraries(Dependencies INTERFACE
            -ldl
            -lasound
            -lportaudio
            )

    target_compile_definitions(Dependencies INTERFACE
            SPELUNKY_PSP_PLATFORM_LINUX
            SPELUNKY_PSP_PLATFORM_DESKTOP
            )
endmacro()

macro(spelunky_psp_post_build_miyoo)
endmacro()
