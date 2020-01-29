add_library(log.c STATIC
        src/log.c
        interface/logger/log.h
)

target_include_directories(log.c PRIVATE interface/logger INTERFACE interface)

if(SPELUNKY_PSP_PLATFORM_LINUX)
    # Don't use ANSI color codes on PSP, as when printed in PPSSPP's output, they break log message.
    target_compile_definitions(log.c PRIVATE LOG_USE_COLOR SPELUNKY_PSP_PLATFORM_LINUX)
endif()
