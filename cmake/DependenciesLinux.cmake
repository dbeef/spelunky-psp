macro(add_linux_dependencies)
    include(FindSDL2)
    add_library(SDL_2_XX INTERFACE)
    target_link_libraries(SDL_2_XX INTERFACE SDL2::Core)

    add_library(Dependencies INTERFACE)
    target_link_libraries(Dependencies INTERFACE
            -lGL
            -lX11
            -ldl
            )

    target_compile_definitions(Dependencies INTERFACE
            SPELUNKY_PSP_PLATFORM_LINUX
            SPELUNKY_PSP_PLATFORM_DESKTOP
            )
endmacro()

macro(spelunky_psp_post_build_linux)
endmacro()
