project(resource-compiler)

add_executable(resource-compiler src/Main.cpp)
set_target_properties(resource-compiler PROPERTIES
        CXX_STANDARD
        14
        CXX_EXTENSIONS
        OFF
)
