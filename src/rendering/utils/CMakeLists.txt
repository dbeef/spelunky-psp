project(RenderingUtils)

add_library(RenderingUtils STATIC
        src/Quad.cpp
        interface/Point2D.hpp
        interface/Mesh.hpp
        interface/Quad.hpp
        interface/Vertex.hpp
        interface/IndexType.hpp
        )

target_include_directories(RenderingUtils
        PRIVATE interface
        INTERFACE interface
)

set_target_properties(RenderingUtils PROPERTIES
        CXX_STANDARD
        14
)

target_link_libraries(RenderingUtils
    PRIVATE
        Logger
        GraphicsUtils
        cjson
        )
