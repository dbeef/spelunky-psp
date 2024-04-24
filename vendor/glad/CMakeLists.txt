if (SPELUNKY_PSP_PLATFORM_LINUX OR
    SPELUNKY_PSP_PLATFORM_WINDOWS OR
    SPELUNKY_PSP_PLATFORM_DARWIN)
    add_subdirectory(glad_opengl_1.3)
elseif(SPELUNKY_PSP_PLATFORM_PSP)
    add_subdirectory(glad_dummy)
elseif(SPELUNKY_PSP_PLATFORM_ANDROID)
    add_subdirectory(glad_opengl_es_1.0)
else()
    message(FATAL_ERROR "Could not match platform")
endif ()
