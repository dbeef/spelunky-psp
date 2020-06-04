project(GameLoop)

add_library(GameLoop STATIC
        src/game-loop/GameLoop.cpp
        src/game-loop/GameLoopMainMenuState.cpp
        src/game-loop/GameLoopPlayingState.cpp
        src/game-loop/GameLoopStartedState.cpp
        src/game-loop/GameLoopLevelSummaryState.cpp
        src/main-dude/MainDude.cpp
        src/main-dude/states/MainDudeStandingState.cpp
        src/main-dude/states/MainDudeCliffHangingState.cpp
        src/main-dude/states/MainDudeClimbingLadderState.cpp
        src/main-dude/states/MainDudeLevelSummaryState.cpp
        src/main-dude/states/MainDudeExitingState.cpp
        src/main-dude/states/MainDudeThrowingState.cpp
        src/main-dude/states/MainDudeRunningState.cpp
        src/main-dude/states/MainDudePushingState.cpp
        src/main-dude/states/MainDudeCrawlingState.cpp
        src/main-dude/states/MainDudeDuckingState.cpp
        src/main-dude/states/MainDudeFallingState.cpp
        src/main-dude/states/MainDudeJumpingState.cpp
        src/components/PhysicsComponent.cpp
        src/components/QuadComponent.cpp
        src/components/AnimationComponent.cpp
        interface/GameLoop.hpp
        interface/GameLoopBaseState.hpp
        interface/GameLoopMainMenuState.hpp
        interface/GameLoopPlayingState.hpp
        interface/GameLoopStartedState.hpp
        interface/GameLoopLevelSummaryState.hpp
        include/main-dude/MainDude.hpp
        include/game-objects/GameObject.hpp
        include/game-objects/MainLogo.hpp
        include/game-objects/QuitSign.hpp
        include/game-objects/StartSign.hpp
        include/game-objects/ScoresSign.hpp
        include/game-objects/TutorialSign.hpp
        include/game-objects/CopyrightsSign.hpp
        include/game-objects/TextBuffer.hpp
        include/game-objects/HUD.hpp
        include/components/PhysicsComponent.hpp
        include/components/QuadComponent.hpp
        include/components/AnimationComponent.hpp
        include/main-dude/states/MainDudeBaseState.hpp
        include/main-dude/states/MainDudeClimbingLadderState.hpp
        include/main-dude/states/MainDudeLevelSummaryState.hpp
        include/main-dude/states/MainDudeRunningState.hpp
        include/main-dude/states/MainDudeStandingState.hpp
        include/main-dude/states/MainDudePushingState.hpp
        include/main-dude/states/MainDudeCrawlingState.hpp
        include/main-dude/states/MainDudeDuckingState.hpp
        include/main-dude/states/MainDudeFallingState.hpp
        include/main-dude/states/MainDudeJumpingState.hpp
        include/main-dude/states/MainDudeThrowingState.hpp
        include/main-dude/states/MainDudeExitingState.hpp
        include/main-dude/states/MainDudeCliffHangingState.hpp
)

target_include_directories(GameLoop
        PRIVATE include interface
        INTERFACE interface
)

set_target_properties(GameLoop PROPERTIES
        CXX_STANDARD
        14
)

target_link_libraries(GameLoop
    PRIVATE
        glad
        Logger
        Camera
        Input
        GraphicsUtils
        Renderer
        Collisions
        Video
)
