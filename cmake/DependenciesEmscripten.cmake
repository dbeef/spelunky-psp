macro(add_emscripten_dependencies)

    find_package(SDL2 CONFIG REQUIRED)
    # FIXME: Audio target
    # find_package(SDL2_mixer REQUIRED)

    add_library(SDL_2_XX INTERFACE)
    target_link_libraries(SDL_2_XX INTERFACE
        ${SDL2_MIXER_LIBRARIES}
        SDL2::SDL2
        SDL2::SDL2main
    )

    target_include_directories(SDL_2_XX INTERFACE
        # As include paths are used with "SDL2/" prefix:
        ${SDL2_INCLUDE_DIRS}/../
        ${SDL2_MIXER_INCLUDE_DIRS}/../
    )

    add_library(Dependencies INTERFACE)
    target_link_libraries(Dependencies INTERFACE SDL_2_XX)
    target_compile_definitions(Dependencies INTERFACE
            SPELUNKY_PSP_PLATFORM_EMSCRIPTEN
            SPELUNKY_PSP_PLATFORM_DESKTOP
    )
endmacro()

macro(spelunky_psp_post_build_emscripten)
endmacro()
