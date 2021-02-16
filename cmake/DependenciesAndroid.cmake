macro(add_android_dependencies)

    foreach(PATH ${CMAKE_PREFIX_PATH})
        set(SDL2_DIR "${PATH}/SDL2/lib/cmake/SDL2/")
        message(STATUS "Checking directory for SDL2: ${SDL2_DIR}")
        find_package(SDL2)
        if (SDL2_FOUND)
            break()
        endif ()
    endforeach()

    if (NOT SDL2_FOUND)
        message(FATAL "Failed to find SDL2. Make sure you passed it through CMAKE_PREFIX_PATH.")
    endif ()

    add_library(SDL_2_XX INTERFACE)
    target_link_libraries(SDL_2_XX INTERFACE
		SDL2::SDL2-static
		SDL2::SDL2main
    )

    add_library(Dependencies INTERFACE)
    target_link_libraries(Dependencies INTERFACE
            -lGLESv1_CM
            -landroid
            -llog
            -ldl
            -lz
            -lEGL
    )

    target_compile_definitions(Dependencies INTERFACE
            SPELUNKY_PSP_PLATFORM_ANDROID
    )
endmacro()

macro(spelunky_psp_post_build_android)
    # Copy libhidapi that is supposed to be bundled with SDL2:
    add_custom_command(TARGET Spelunky_PSP POST_BUILD
        COMMAND echo Copying libhidapi.so...
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${SDL2_DIR}/../../libhidapi.so"
                "$<TARGET_FILE_DIR:Spelunky_PSP>/libhidapi.so"
    )
endmacro()
