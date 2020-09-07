macro(add_miyoo_dependencies)

    find_package(SDL2 REQUIRED)
    
    add_library(SDL_1_XX INTERFACE)
    target_link_libraries(SDL_1_XX INTERFACE
            #SDL2::SDL2-static
            SDL2::SDL2
            SDL2::SDL2main
        )

    add_library(Dependencies INTERFACE)
    target_link_libraries(Dependencies INTERFACE
            -lc
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
