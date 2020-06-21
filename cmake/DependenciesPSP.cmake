# FIXME: This macro probably defines target that links to more libraries than actually needed.
macro(add_psp_dependencies)
    add_library(SDL_2_XX INTERFACE)
    target_link_libraries(SDL_2_XX
        INTERFACE
            -lSDL2
        )

    add_library(Dependencies INTERFACE)
    target_link_libraries(Dependencies INTERFACE
            -L${PSPDEV}/psp/lib
            -lg
            -lstdc++
            -lc
            -lGLU
            -lglut
            -lz
            -lm
            -lGL
            -lpspvfpu
            -L${PSPDEV}/psp/sdk/lib
            -L${PSPDEV}/psp
            -lpspdebug
            -lpspgu
            -lpspctrl
            -lpspge
            -lpspirkeyb
            -lpsppower
            -lpsppower_driver
            -lpspdisplay
            -lpsphprm
            -lpspsdk
            -lpsprtc
            -lpspaudio
            -lpsputility
            -lpspnet_inet
            -lc
            -lpspuser
            )

    target_compile_definitions(Dependencies INTERFACE
            SPELUNKY_PSP_PLATFORM_PSP
            -Dmain=SDL2_main
            )

endmacro()
