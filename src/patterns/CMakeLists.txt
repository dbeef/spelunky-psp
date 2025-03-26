project(Patterns)

add_library(Patterns INTERFACE)
target_include_directories(Patterns INTERFACE interface)
target_link_libraries(Patterns INTERFACE Logger)
