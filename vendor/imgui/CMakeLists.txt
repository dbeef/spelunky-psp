add_library(imgui STATIC
    imgui.cpp
    imgui.h
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    backends/imgui_impl_sdl2.cpp
    backends/imgui_impl_sdl2.h
    backends/imgui_impl_opengl2.cpp
    backends/imgui_impl_opengl2.h
)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_LIST_DIR} backends)
target_link_libraries(imgui PRIVATE glad SDL_2_XX)
set_target_properties(imgui PROPERTIES CXX_STANDARD 17)
