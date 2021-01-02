project(Renderer)

add_library(Renderer STATIC
        src/Renderer.cpp
        src/Quad.cpp
        interface/Renderer.hpp
        interface/RenderingLayer.hpp
        interface/Point2D.hpp
        interface/RenderEntity.hpp
        interface/Mesh.hpp
        interface/Quad.hpp
        interface/Vertex.hpp
        interface/IndexType.hpp
        )

target_include_directories(Renderer
        PRIVATE include interface
        INTERFACE interface
)

set_target_properties(Renderer PROPERTIES
        CXX_STANDARD
        17
)

target_link_libraries(Renderer
    PUBLIC
        Patterns
        TextureBank
    PRIVATE
        Logger
        GraphicsUtils
        )

if (SPELUNKY_PSP_PLATFORM_LINUX)
    target_link_libraries(Renderer PRIVATE dl)
    target_compile_definitions(Renderer PRIVATE SPELUNKY_PSP_PLATFORM_LINUX)
elseif(SPELUNKY_PSP_PLATFORM_PSP)
    target_compile_definitions(Renderer PRIVATE SPELUNKY_PSP_PLATFORM_PSP)
endif()
