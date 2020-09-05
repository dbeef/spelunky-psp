macro(add_android_dependencies)

	find_package(SDL2 REQUIRED)

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
    # TODO: Call gradlew to create .apk
endmacro()
