add_library(glad INTERFACE)
target_include_directories(glad INTERFACE include)

# For platform defines:
target_link_libraries(glad INTERFACE Dependencies)
