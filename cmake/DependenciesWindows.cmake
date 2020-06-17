macro(add_windows_dependencies)
    find_package(SDL2 REQUIRED)

    add_library(Dependencies INTERFACE)

    target_compile_definitions(Dependencies INTERFACE
            SPELUNKY_PSP_PLATFORM_WINDOWS
            SPELUNKY_PSP_PLATFORM_DESKTOP
            )
endmacro()

macro(spelunky_psp_post_build_windows)
    list(GET SDL2_LIBRARY 0 _SDL2_LIBRARY)
    get_filename_component(SDL2_DIR ${_SDL2_LIBRARY} DIRECTORY)
    if(NOT EXISTS ${SDL2_DIR}/SDL2.dll)
        message(FATAL_ERROR "Missing SDL2.dll dependency in ${SDL2_DIR}")
    endif() 
    add_custom_command(TARGET Spelunky_PSP POST_BUILD
        COMMAND echo Copying SDL2.dll...
        COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                "${SDL2_DIR}/SDL2.dll"
                "$<TARGET_FILE_DIR:Spelunky_PSP>/SDL2.dll"
        )
    install(FILES "${SDL2_DIR}/SDL2.dll" 
        CONFIGURATIONS Release
        DESTINATION Release/bin)
    install(FILES "${SDL2_DIR}/SDL2.dll" 
        CONFIGURATIONS Debug
        DESTINATION Debug/bin)

    unset(_SDL2_LIBRARY)
    unset(SDL2_DIR)
endmacro()
