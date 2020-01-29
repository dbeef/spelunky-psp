add_library(log.c STATIC
        src/log.c
        interface/log.h
)

target_include_directories(log.c PRIVATE interface INTERFACE interface)
