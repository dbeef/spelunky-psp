# FIXME: This macro probably defines target that links to more libraries than actually needed.
macro(add_psp_dependencies)
    add_library(SDL_1_XX INTERFACE)
    target_link_libraries(SDL_1_XX INTERFACE
        -lSDL_mixer
        -lSDLmain
        -lSDL
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
            -lvorbisidec
            -lmikmod
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
            -Dmain=SDL_main
            )

endmacro()

macro(spelunky_psp_post_build_psp)
    add_custom_command(
            TARGET Spelunky_PSP
            POST_BUILD COMMAND
            "psp-strip" "$<TARGET_FILE:Spelunky_PSP>"
            COMMENT "Stripping binary"
    )

    create_pbp_file(TARGET Spelunky_PSP
            ICON_PATH ${ASSETS_PATH}/metadata/icon.png
            BACKGROUND_PATH ${ASSETS_PATH}/metadata/background.png
    )
endmacro()
