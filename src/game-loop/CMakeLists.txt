project(GameLoop)

add_library(GameLoop STATIC
        src/GameLoop.cpp
        src/main-dude/MainDude.cpp
        src/main-dude/states/MainDudeStanding.cpp
        src/main-dude/states/MainDudeRunning.cpp
        src/main-dude/states/MainDudePushing.cpp
        src/main-dude/states/MainDudeCrawlingState.cpp
        src/main-dude/states/MainDudeDuckingState.cpp
        src/main-dude/states/MainDudeFallingState.cpp
        src/main-dude/states/MainDudeJumpingState.cpp
        src/components/PhysicsComponent.cpp
        src/components/QuadComponent.cpp
        src/components/InputComponent.cpp
        src/components/AnimationComponent.cpp
        interface/GameLoop.hpp
        interface/GameState.hpp
        include/main-dude/MainDude.hpp
        include/GameObject.hpp
        include/components/PhysicsComponent.hpp
        include/components/QuadComponent.hpp
        include/components/InputComponent.hpp
        include/components/AnimationComponent.hpp
        include/main-dude/MainDudeSpritesheetFrames.hpp
        include/main-dude/states/MainDudeBaseState.hpp
        include/main-dude/states/MainDudeRunning.hpp
        include/main-dude/states/MainDudeStanding.hpp
        include/main-dude/states/MainDudePushing.hpp
        include/main-dude/states/MainDudeCrawlingState.hpp
        include/main-dude/states/MainDudeDuckingState.hpp
        include/main-dude/states/MainDudeFallingState.hpp
        include/main-dude/states/MainDudeJumpingState.hpp
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
