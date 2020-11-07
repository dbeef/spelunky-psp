macro(add_linux_dependencies)

    add_library(SDL_1_XX INTERFACE)
    target_link_libraries(SDL_1_XX INTERFACE
    /home/dbeef/MiyooSDK/miyoo/arm-miyoo-linux-uclibcgnueabi/sysroot/usr/lib/libSDL-1.2.so.0.11.4
    /home/dbeef/MiyooSDK/miyoo/arm-miyoo-linux-uclibcgnueabi/sysroot/usr/lib/libSDLmain.a
    )

    add_library(Dependencies INTERFACE)
    target_link_libraries(Dependencies INTERFACE
            -ldl -lpthread -lm
            )

    target_compile_definitions(Dependencies INTERFACE
            SPELUNKY_PSP_PLATFORM_LINUX
            SPELUNKY_PSP_PLATFORM_DESKTOP
        )
endmacro()

macro(spelunky_psp_post_build_linux)
endmacro()
