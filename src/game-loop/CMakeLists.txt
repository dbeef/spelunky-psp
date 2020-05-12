project(GameLoop)

add_library(GameLoop STATIC
        src/GameLoop.cpp
        src/main-dude/MainDude.cpp
        src/main-dude/states/MainDudeStanding.cpp
        src/main-dude/states/MainDudeRunning.cpp
        src/components/PhysicsComponent.cpp
        src/components/QuadComponent.cpp
        src/components/InputComponent.cpp
        interface/GameLoop.hpp
        interface/GameState.hpp
        include/main-dude/MainDude.hpp
        include/GameObject.hpp
        include/components/PhysicsComponent.hpp
        include/components/QuadComponent.hpp
        include/components/InputComponent.hpp
        include/main-dude/MainDudeSpritesheetFrames.hpp
        include/main-dude/states/MainDudeBaseState.hpp
        include/main-dude/states/MainDudeRunning.hpp
        include/main-dude/states/MainDudeStanding.hpp
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
