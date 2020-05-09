macro(add_windows_dependencies)
    find_package(SDL REQUIRED)
    set(SDL_INCLUDE_DIR "${SDL_INCLUDE_DIR}/../")

    # Creating an explicit SDL target, for others
    add_library(SDL_1_XX INTERFACE)
    target_include_directories(SDL_1_XX INTERFACE ${SDL_INCLUDE_DIR})
    target_link_libraries(SDL_1_XX INTERFACE ${SDL_LIBRARY})

    add_library(Dependencies INTERFACE)
    target_link_libraries(Dependencies INTERFACE SDL_1_XX)

    target_compile_definitions(Dependencies INTERFACE
            SPELUNKY_PSP_PLATFORM_WINDOWS
            )
endmacro()

macro(spelunky_psp_post_build_windows)
endmacro()
