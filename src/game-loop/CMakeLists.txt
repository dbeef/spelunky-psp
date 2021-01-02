project(GameLoop)

add_library(GameLoop STATIC
        src/game-loop/GameLoop.cpp
        src/game-loop/GameLoopMainMenuState.cpp
        src/game-loop/GameLoopPlayingState.cpp
        src/game-loop/GameLoopStartedState.cpp
        src/game-loop/GameLoopLevelSummaryState.cpp
        src/game-loop/GameLoopScoresState.cpp

        interface/game-loop/GameLoop.hpp
        interface/game-loop/GameLoopBaseState.hpp
        interface/game-loop/GameLoopMainMenuState.hpp
        interface/game-loop/GameLoopPlayingState.hpp
        interface/game-loop/GameLoopStartedState.hpp
        interface/game-loop/GameLoopLevelSummaryState.hpp
        interface/game-loop/GameLoopScoresState.hpp

        src/main-dude/MainDudeComponent.cpp
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
        src/main-dude/states/MainDudeLookingUpState.cpp
        src/main-dude/states/MainDudeRunningLookingUpState.cpp
        src/main-dude/states/MainDudeDeadState.cpp
        src/main-dude/states/MainDudeStunnedState.cpp
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
        include/main-dude/states/MainDudeLookingUpState.hpp
        include/main-dude/states/MainDudeRunningLookingUpState.hpp
        include/main-dude/states/MainDudeDeadState.hpp
        include/main-dude/states/MainDudeStunnedState.hpp
        include/main-dude/MainDudeEvent.hpp

        src/components/specialized/HudOverlayComponent.cpp
        src/components/specialized/PauseOverlayComponent.cpp
        src/components/specialized/DeathOverlayComponent.cpp
        src/components/specialized/LevelSummaryOverlayComponent.cpp
        src/components/specialized/LevelSummaryTracker.cpp
        src/components/generic/QuadComponent.cpp
        src/components/generic/TextComponent.cpp
        src/components/generic/AnimationComponent.cpp
        src/components/generic/PhysicsComponent.cpp
        include/components/generic/PositionComponent.hpp
        include/components/generic/QuadComponent.hpp
        include/components/generic/PhysicsComponent.hpp
        include/components/generic/AnimationComponent.hpp
        include/components/generic/MeshComponent.hpp
        include/components/generic/SortRenderingLayersComponent.hpp
        include/components/specialized/MainDudeComponent.hpp
        include/components/specialized/LevelSummaryTracker.hpp
        include/components/specialized/HudOverlayComponent.hpp
        include/components/specialized/PauseOverlayComponent.hpp
        include/components/specialized/DeathOverlayComponent.hpp
        include/components/specialized/LevelSummaryOverlayComponent.hpp

        include/system/System.hpp
        include/system/RenderingSystem.hpp
        include/system/PhysicsSystem.hpp
        include/system/ScriptingSystem.hpp
        include/system/AnimationSystem.hpp

        src/populator/Populator.cpp
        include/populator/Populator.hpp
        include/populator/Spawner.hpp

        src/other/Inventory.cpp
        interface/other/PhysicsComponentType.hpp
        interface/other/Inventory.hpp
        interface/other/InventoryEvent.hpp
        include/other/LootCollectedEvent.hpp

        include/prefabs/props/StartSign.hpp
        include/prefabs/props/ScoresSign.hpp
        include/prefabs/props/ResetSign.hpp
        include/prefabs/props/TutorialSign.hpp
        include/prefabs/props/QuitSign.hpp
        include/prefabs/props/MainLogo.hpp
        include/prefabs/props/CopyrightsSign.hpp
        include/prefabs/main-dude/MainDude.hpp
        include/prefabs/ui/PauseOverlay.hpp
        include/prefabs/ui/ScoresOverlay.hpp
        include/prefabs/ui/DeathOverlay.hpp
        include/prefabs/ui/LevelSummaryOverlay.hpp
        include/prefabs/ui/HudOverlay.hpp

        src/prefabs/props/QuitSign.cpp
        src/prefabs/props/ResetSign.cpp
        src/prefabs/props/ScoresSign.cpp
        src/prefabs/props/StartSign.cpp
        src/prefabs/props/CopyrightsSign.cpp
        src/prefabs/props/MainLogo.cpp
        src/prefabs/props/TutorialSign.cpp
        src/prefabs/main-dude/MainDude.cpp
        src/prefabs/ui/PauseOverlay.cpp
        src/prefabs/ui/ScoresOverlay.cpp
        src/prefabs/ui/DeathOverlay.cpp
        src/prefabs/ui/LevelSummaryOverlay.cpp
        src/prefabs/ui/HudOverlay.cpp
        include/prefabs/ui/Text.hpp src/prefabs/ui/Text.cpp include/prefabs/collectibles/SingleGoldBar.hpp src/prefabs/collectibles/SingleGoldBar.cpp include/prefabs/collectibles/TripleGoldBar.hpp src/prefabs/collectibles/TripleGoldBar.cpp include/prefabs/collectibles/BigGem.hpp src/prefabs/collectibles/BigGem.cpp include/prefabs/items/Rock.hpp src/prefabs/items/Rock.cpp include/prefabs/items/Jar.hpp src/prefabs/items/Jar.cpp include/prefabs/items/Chest.hpp src/prefabs/items/Chest.cpp include/components/generic/CollectibleComponent.hpp include/system/CollectibleSystem.hpp src/system/RenderingSystem.cpp include/prefabs/ui/HudIcon.hpp src/prefabs/ui/HudIcon.cpp src/system/PhysicsSystem.cpp src/system/CollectibleSystem.cpp src/system/AnimationSystem.cpp src/system/ScriptingSystem.cpp)

target_include_directories(GameLoop
        PRIVATE include interface
        INTERFACE interface
)

set_target_properties(GameLoop PROPERTIES
        CXX_STANDARD
        17
)

target_link_libraries(GameLoop
    PUBLIC
        Camera
        Viewport
        EntityRegistry
    PRIVATE
        Logger
        Input
        RenderingTypes
        Collisions
        Level
        Patterns
        Audio
        glad
        GraphicsUtils
)
