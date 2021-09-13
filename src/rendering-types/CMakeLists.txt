project(RenderingTypes)

add_library(RenderingTypes STATIC
        src/Quad.cpp
        interface/RenderingLayer.hpp
        interface/Point2D.hpp
        interface/RenderEntity.hpp
        interface/Mesh.hpp
        interface/Quad.hpp
        interface/Vertex.hpp
        interface/IndexType.hpp
        interface/CameraType.hpp
        interface/Vector2D.hpp
)

target_include_directories(RenderingTypes
        PRIVATE include interface
        INTERFACE interface
)

set_target_properties(RenderingTypes PROPERTIES
        CXX_STANDARD
        17
)

target_link_libraries(RenderingTypes
    PUBLIC
        Patterns
        TextureBank
    PRIVATE
        Logger
        GraphicsUtils
        )

if (SPELUNKY_PSP_PLATFORM_LINUX)
    target_link_libraries(RenderingTypes PRIVATE dl)
    target_compile_definitions(RenderingTypes PRIVATE SPELUNKY_PSP_PLATFORM_LINUX)
elseif(SPELUNKY_PSP_PLATFORM_PSP)
    target_compile_definitions(RenderingTypes PRIVATE SPELUNKY_PSP_PLATFORM_PSP)
endif()
