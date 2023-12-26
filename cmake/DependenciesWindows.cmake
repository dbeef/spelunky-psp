macro(add_windows_dependencies)
    find_package(SDL REQUIRED)
    find_package(SDL_mixer REQUIRED)

    set(SDL_INCLUDE_DIR "${SDL_INCLUDE_DIR}/../")

    # Creating an explicit SDL target, for others
    add_library(SDL_1_XX INTERFACE)
    target_include_directories(SDL_1_XX INTERFACE ${SDL_INCLUDE_DIR} ${SDL_MIXER_INCLUDE_DIRS})
    target_link_libraries(SDL_1_XX INTERFACE ${SDL_LIBRARY} ${SDL_MIXER_LIBRARIES})

    add_library(Dependencies INTERFACE)

    target_compile_definitions(Dependencies INTERFACE
            SPELUNKY_PSP_PLATFORM_WINDOWS
            SPELUNKY_PSP_PLATFORM_DESKTOP
            SPELUNKY_PSP_WITH_IMGUI
    )
    set(SPELUNKY_PSP_WITH_IMGUI TRUE)
endmacro()

macro(spelunky_psp_post_build_windows)
    list(GET SDL_LIBRARY 0 _SDL_LIBRARY)
    get_filename_component(SDL_DIR ${_SDL_LIBRARY} DIRECTORY)

    if(NOT EXISTS ${SDL_DIR}/SDL.dll)
        message(FATAL_ERROR "Missing SDL.dll dependency in ${SDL_DIR}")
    endif() 

    if(NOT EXISTS ${SDL_DIR}/SDL_mixer.dll)
        message(FATAL_ERROR "Missing SDL_mixer.dll dependency in ${SDL_DIR}")
    endif()

    add_custom_command(TARGET Spelunky_PSP POST_BUILD
        COMMAND echo Copying SDL.dll...
        COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                "${SDL_DIR}/SDL.dll"
                "$<TARGET_FILE_DIR:Spelunky_PSP>/SDL.dll"
        )

    add_custom_command(TARGET Spelunky_PSP POST_BUILD
        COMMAND echo Copying SDL_mixer.dll...
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${SDL_DIR}/SDL_mixer.dll"
                "$<TARGET_FILE_DIR:Spelunky_PSP>/SDL_mixer.dll"
        )

    install(FILES "${SDL_DIR}/SDL.dll" 
        CONFIGURATIONS Release
        DESTINATION Release/bin)
    install(FILES "${SDL_DIR}/SDL.dll" 
        CONFIGURATIONS Debug
        DESTINATION Debug/bin)

    install(FILES "${SDL_DIR}/SDL_mixer.dll"
        CONFIGURATIONS Release
        DESTINATION Release/bin)
    install(FILES "${SDL_DIR}/SDL_mixer.dll"
        CONFIGURATIONS Debug
        DESTINATION Debug/bin)

    unset(_SDL_LIBRARY)
    unset(SDL_DIR)
endmacro()
