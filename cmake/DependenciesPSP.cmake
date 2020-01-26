# FIXME: This macro probably defines target that links to more libraries than actually needed.
macro(add_psp_dependencies)
    add_library(Dependencies INTERFACE)

    target_link_libraries(Dependencies INTERFACE
            -L${PSPDEV}/psp/lib
            -lSDLmain
            -lSDL
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
            -Dmain=SDL_main
            )

endmacro()


macro(spelunky_psp_post_build_psp)

    add_custom_command(TARGET Spelunky_PSP POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
            $<TARGET_FILE_DIR:Spelunky_PSP>/psp_artifact
            COMMENT "Creating psp_artifact directory."
            )

    add_custom_command(TARGET Spelunky_PSP POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:Spelunky_PSP>
            $<TARGET_FILE_DIR:Spelunky_PSP>/psp_artifact
            COMMENT "Copying ELF to psp_arfitact directory."
            )

    add_custom_command(
            TARGET Spelunky_PSP
            POST_BUILD COMMAND
            "psp-fixup-imports" "$<TARGET_FILE_DIR:Spelunky_PSP>/psp_artifact/Spelunky_PSP"
            COMMENT "Calling psp-fixup-imports"
    )

    add_custom_command(
            TARGET Spelunky_PSP
            POST_BUILD COMMAND
            "mksfoex" "-d" "MEMSIZE=1" "$<TARGET_FILE_DIR:Spelunky_PSP>/psp_artifact/Spelunky_PSP" "PARAM.SFO"
            COMMENT "Calling mksfoex"
    )

    add_custom_command(
            TARGET Spelunky_PSP
            POST_BUILD COMMAND
            "pack-pbp" "SpelunkyPSP.PBP" "PARAM.SFO" "${ASSETS_PATH}/icon.png" "NULL" "NULL"
            "${ASSETS_PATH}/background.png" "NULL" "$<TARGET_FILE_DIR:Spelunky_PSP>/psp_artifact/Spelunky_PSP" "NULL"
            COMMENT "Calling pack-pbp"
    )

    add_custom_command(
            TARGET Spelunky_PSP
            POST_BUILD COMMAND
            ${CMAKE_COMMAND} -E cmake_echo_color --cyan "You can call ppsspp Spelunky_PSP.PBP now."
    )

endmacro()
