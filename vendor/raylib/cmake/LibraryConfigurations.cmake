if (${PLATFORM} MATCHES "Desktop")
    set(PLATFORM_CPP "PLATFORM_DESKTOP")
    
    if (APPLE)
        # Need to force OpenGL 3.3 on OS X
        # See: https://github.com/raysan5/raylib/issues/341
        set(GRAPHICS "GRAPHICS_API_OPENGL_33")
        find_library(OPENGL_LIBRARY OpenGL)
        set(LIBS_PRIVATE ${OPENGL_LIBRARY})
        link_libraries("${LIBS_PRIVATE}")
        if (NOT CMAKE_SYSTEM STRLESS "Darwin-18.0.0")
            add_definitions(-DGL_SILENCE_DEPRECATION)
            MESSAGE(AUTHOR_WARNING "OpenGL is deprecated starting with macOS 10.14 (Mojave)!")
        endif ()
    elseif (WIN32)
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
        set(LIBS_PRIVATE ${LIBS_PRIVATE} winmm)
    else ()
        find_library(pthread NAMES pthread)
        find_package(OpenGL QUIET)
        if ("${OPENGL_LIBRARIES}" STREQUAL "")
            set(OPENGL_LIBRARIES "GL")
        endif ()
        
        if ("${CMAKE_SYSTEM_NAME}" MATCHES "(Net|Open)BSD")
            find_library(OSS_LIBRARY ossaudio)
        endif ()
        
        set(LIBS_PRIVATE m pthread ${OPENGL_LIBRARIES} ${OSS_LIBRARY})
    endif ()

elseif (${PLATFORM} MATCHES "Web")
    set(PLATFORM_CPP "PLATFORM_WEB")
    set(GRAPHICS "GRAPHICS_API_OPENGL_ES2")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 --profiling")
    set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")

elseif (${PLATFORM} MATCHES "Android")
    set(PLATFORM_CPP "PLATFORM_ANDROID")
    set(GRAPHICS "GRAPHICS_API_OPENGL_ES2")
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    
    add_definitions(-DANDROID -D__ANDROID_API__=21)
    include_directories(external/android/native_app_glue)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--exclude-libs,libatomic.a -Wl,--build-id -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -Wl,--warn-shared-textrel -Wl,--fatal-warnings -uANativeActivity_onCreate")
    
    find_library(OPENGL_LIBRARY OpenGL)
    set(LIBS_PRIVATE m log android EGL GLESv2 OpenSLES atomic c)

elseif (${PLATFORM} MATCHES "Raspberry Pi")
    set(PLATFORM_CPP "PLATFORM_RPI")
    set(GRAPHICS "GRAPHICS_API_OPENGL_ES2")
    
    add_definitions(-D_DEFAULT_SOURCE)
    
    find_library(GLESV2 brcmGLESv2 HINTS /opt/vc/lib)
    find_library(EGL brcmEGL HINTS /opt/vc/lib)
    find_library(BCMHOST bcm_host HINTS /opt/vc/lib)
    include_directories(/opt/vc/include /opt/vc/include/interface/vmcs_host/linux /opt/vc/include/interface/vcos/pthreads)
    link_directories(/opt/vc/lib)
    set(LIBS_PRIVATE ${GLESV2} ${EGL} ${BCMHOST} pthread rt m dl)

elseif (${PLATFORM} MATCHES "DRM")
    set(PLATFORM_CPP "PLATFORM_DRM")
    set(GRAPHICS "GRAPHICS_API_OPENGL_ES2")
    
    add_definitions(-D_DEFAULT_SOURCE)
    add_definitions(-DEGL_NO_X11)
    add_definitions(-DPLATFORM_DRM)
    
    find_library(GLESV2 GLESv2)
    find_library(EGL EGL)
    find_library(DRM drm)
    find_library(GBM gbm)
    
    if (NOT CMAKE_CROSSCOMPILING)
        include_directories(/usr/include/libdrm)
    endif ()
    set(LIBS_PRIVATE ${GLESV2} ${EGL} ${DRM} ${GBM} pthread m dl)

endif ()

if (${OPENGL_VERSION})
    set(${SUGGESTED_GRAPHICS} "${GRAPHICS}")
    if (${OPENGL_VERSION} MATCHES "3.3")
        set(GRAPHICS "GRAPHICS_API_OPENGL_33")
    elseif (${OPENGL_VERSION} MATCHES "2.1")
        set(GRAPHICS "GRAPHICS_API_OPENGL_21")
    elseif (${OPENGL_VERSION} MATCHES "1.1")
        set(GRAPHICS "GRAPHICS_API_OPENGL_11")
    elseif (${OPENGL_VERSION} MATCHES "ES 2.0")
        set(GRAPHICS "GRAPHICS_API_OPENGL_ES2")
    endif ()
    if ("${SUGGESTED_GRAPHICS}" AND NOT "${SUGGESTED_GRAPHICS}" STREQUAL "${GRAPHICS}")
        message(WARNING "You are overriding the suggested GRAPHICS=${SUGGESTED_GRAPHICS} with ${GRAPHICS}! This may fail")
    endif ()
endif ()

if (NOT GRAPHICS)
    set(GRAPHICS "GRAPHICS_API_OPENGL_33")
endif ()

set(LIBS_PRIVATE ${LIBS_PRIVATE} ${OPENAL_LIBRARY})

if (${PLATFORM} MATCHES "Desktop")
    set(LIBS_PRIVATE ${LIBS_PRIVATE} glfw)
endif ()
