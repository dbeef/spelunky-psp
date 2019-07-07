include(cmake/DependenciesLinux.cmake)
include(cmake/DependenciesPSP.cmake)

macro(spelunky_psp_detect_platform)

    if (DEFINED SPELUNKY_PSP_PLATFORM)
        # Nothing to do - SPELUNKY_PSP_PLATFORM was already defined in the toolchain file.
    elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
        set(SPELUNKY_PSP_PLATFORM "Linux")
    else ()
        message(FATAL_ERROR "Your platform (${CMAKE_SYSTEM_NAME}) is not supported")
    endif ()

    message(STATUS "Target platform is: ${SPELUNKY_PSP_PLATFORM}")
endmacro()

macro(spelunky_psp_add_platform_dependencies)
    if (${SPELUNKY_PSP_PLATFORM} STREQUAL PSP)
        add_psp_dependencies()
    elseif (${SPELUNKY_PSP_PLATFORM} STREQUAL Linux)
        add_linux_dependencies()
    endif ()
endmacro()

macro(spelunky_psp_post_build)
    if (${SPELUNKY_PSP_PLATFORM} STREQUAL PSP)
        spelunky_psp_post_build_psp()
    elseif (${SPELUNKY_PSP_PLATFORM} STREQUAL Linux)
        spelunky_psp_post_build_linux()
    endif ()
endmacro()
