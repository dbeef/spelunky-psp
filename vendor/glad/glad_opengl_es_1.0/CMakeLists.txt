add_library(glad STATIC
        src/glad.c
        include/glad/glad.h
        include/KHR/khrplatform.h
        )

target_include_directories(glad
        PUBLIC include
)

# For platform defines:
target_link_libraries(glad PUBLIC Dependencies)
