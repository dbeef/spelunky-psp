project(GameLoop)

add_library(GameLoop STATIC
        src/GameLoop.cpp
        src/MainDude.cpp
        src/components/PhysicsComponent.cpp
        src/components/InputComponent.cpp
        interface/GameLoop.hpp
        interface/GameState.hpp
        include/MainDude.hpp
        include/GameObject.hpp
        include/components/PhysicsComponent.hpp
        include/components/InputComponent.hpp
)

target_include_directories(GameLoop
        PRIVATE include interface
        INTERFACE interface
)

set_target_properties(GameLoop PROPERTIES
        CXX_STANDARD
        14
)

target_link_libraries(GameLoop PRIVATE
        glad
        stb
        Logger
        glm
        Camera
        Input
        GraphicsUtils
        Renderer
        Collisions
)
