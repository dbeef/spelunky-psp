macro(add_darwin_dependencies)

    find_package(SDL REQUIRED)
    find_package(SDL_mixer REQUIRED)

    add_library(SDL_1_XX INTERFACE)
    target_link_libraries(SDL_1_XX INTERFACE
        ${SDLMAIN_LIBRARY}
        SDL::SDL
        ${SDL_MIXER_LIBRARIES}
    )
    target_include_directories(SDL_1_XX INTERFACE
        # As include paths are used with "SDL/" prefix:
        ${SDL_INCLUDE_DIRS}/../
        ${SDL_MIXER_INCLUDE_DIRS}
    )

    add_library(Dependencies INTERFACE)
    target_link_libraries(Dependencies INTERFACE SDL_1_XX)
    target_compile_definitions(Dependencies INTERFACE
            SPELUNKY_PSP_PLATFORM_DARWIN
            SPELUNKY_PSP_PLATFORM_DESKTOP
    )
endmacro()

macro(spelunky_psp_post_build_darwin)
endmacro()
