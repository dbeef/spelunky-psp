macro(add_psp_dependencies)
    add_library(PSP_SDK INTERFACE)
    target_link_libraries(PSP_SDK INTERFACE
        -L${PSPDEV}/psp/sdk/lib
        -lpspirkeyb
        -lpspdebug
        -lpspgu
        -lpspvram
        -lpspvfpu
        -lpspctrl
        -lpspge
        -lpspdisplay
        -lpsphprm
        -lpspaudio
        -lpsppower_driver
        -lpsppower
    )

    add_library(SDL_2_XX INTERFACE)
    target_link_libraries(SDL_2_XX INTERFACE
        -L${PSPDEV}/psp
        -lGL
        -lmodplug
        -lvorbisenc
        -lvorbisfile
        -lvorbis
        -logg
        -lSDL2_mixer
        -lSDL2main
        -lSDL2
    )

    add_library(Dependencies INTERFACE)
    target_link_libraries(Dependencies INTERFACE
        PSP_SDK
    )
    target_compile_definitions(Dependencies INTERFACE
            SPELUNKY_PSP_PLATFORM_PSP
            -Dmain=SDL_main
    )
endmacro()

macro(spelunky_psp_post_build_psp)
    create_pbp_file(TARGET Spelunky_PSP
            ICON_PATH ${SPELUNKY_PSP_ASSETS_PATH}/development/metadata/icon.png
            BACKGROUND_PATH ${SPELUNKY_PSP_ASSETS_PATH}/development/metadata/background.png
    )
endmacro()
