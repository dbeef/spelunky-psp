project(GameLoop)

add_library(GameLoop STATIC
        # Game loop:

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

        # Main dude:

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

        # Components:

        src/components/generic/ItemCarrierComponent.cpp
        src/components/specialized/HudOverlayComponent.cpp
        src/components/specialized/PauseOverlayComponent.cpp
        src/components/specialized/DeathOverlayComponent.cpp
        src/components/specialized/LevelSummaryOverlayComponent.cpp
        src/components/specialized/LevelSummaryTracker.cpp
        src/components/generic/QuadComponent.cpp
        src/components/generic/TextComponent.cpp
        src/components/generic/AnimationComponent.cpp
        src/components/generic/PhysicsComponent.cpp
        src/components/generic/ItemComponent.cpp
        include/components/generic/OpenableComponent.hpp
        include/components/generic/CollectibleComponent.hpp
        include/components/generic/ScriptingComponent.hpp
        include/components/generic/InputComponent.hpp
        include/components/generic/TimeLimitComponent.hpp
        include/components/generic/ParticleEmitterComponent.hpp
        include/components/generic/ItemCarrierComponent.hpp
        include/components/generic/ItemComponent.hpp
        include/components/generic/HorizontalOrientationComponent.hpp
        include/components/generic/PositionComponent.hpp
        include/components/generic/QuadComponent.hpp
        include/components/generic/PhysicsComponent.hpp
        include/components/generic/AnimationComponent.hpp
        include/components/generic/MeshComponent.hpp
        include/components/generic/ActivableComponent.hpp
        include/components/specialized/MainDudeComponent.hpp
        include/components/specialized/LevelSummaryTracker.hpp
        include/components/specialized/HudOverlayComponent.hpp
        include/components/specialized/PauseOverlayComponent.hpp
        include/components/specialized/DeathOverlayComponent.hpp
        include/components/specialized/LevelSummaryOverlayComponent.hpp

        # Prefabs:

        include/prefabs/npc/Snake.hpp
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
        include/prefabs/ui/Text.hpp
        include/prefabs/collectibles/SingleGoldBar.hpp
        include/prefabs/collectibles/TripleGoldBar.hpp
        include/prefabs/collectibles/BigGem.hpp
        include/prefabs/items/Rock.hpp
        include/prefabs/items/Jar.hpp
        include/prefabs/items/Chest.hpp
        include/prefabs/ui/HudIcon.hpp
        include/prefabs/particles/ItemCollectedParticle.hpp
        include/prefabs/particles/BloodTrailParticle.hpp
        include/prefabs/particles/SmokePuffParticle.hpp
        include/prefabs/particles/RubbleSmallParticle.hpp
        include/prefabs/particles/PoofParticle.hpp
        include/prefabs/particles/FlameTrailParticle.hpp
        include/prefabs/particles/FlameParticle.hpp
        include/prefabs/particles/RopeCollectedParticle.hpp
        include/prefabs/particles/BombCollectedParticle.hpp
        include/prefabs/collectibles/GoldNugget.hpp
        include/prefabs/collectibles/GoldChunk.hpp
        include/prefabs/collectibles/SmallGem.hpp
        include/prefabs/items/Jetpack.hpp
        include/prefabs/items/BombSpawner.hpp
        include/prefabs/items/Bomb.hpp
        include/prefabs/items/Pistol.hpp
        include/prefabs/items/Shotgun.hpp
        include/prefabs/items/RopeSpawner.hpp
        include/prefabs/items/Rope.hpp
        include/prefabs/items/RopeChainElement.hpp
        include/prefabs/items/Whip.hpp
        include/prefabs/items/Crate.hpp
        include/prefabs/items/Cape.hpp
        include/prefabs/effects/Explosion.hpp
        include/prefabs/effects/ShotgunBlast.hpp
        include/prefabs/other/Bullet.hpp

        src/prefabs/npc/Snake.cpp
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
        src/prefabs/ui/Text.cpp
        src/prefabs/collectibles/SingleGoldBar.cpp
        src/prefabs/collectibles/TripleGoldBar.cpp
        src/prefabs/collectibles/BigGem.cpp
        src/prefabs/items/Rock.cpp
        src/prefabs/items/Jar.cpp
        src/prefabs/items/Chest.cpp
        src/prefabs/ui/HudIcon.cpp
        src/prefabs/particles/ItemCollectedParticle.cpp
        src/prefabs/particles/BloodTrailParticle.cpp
        src/prefabs/particles/SmokePuffParticle.cpp
        src/prefabs/particles/RubbleSmallParticle.cpp
        src/prefabs/particles/FlameTrailParticle.cpp
        src/prefabs/particles/PoofParticle.cpp
        src/prefabs/particles/FlameParticle.cpp
        src/prefabs/particles/RopeCollectedParticle.cpp
        src/prefabs/particles/BombCollectedParticle.cpp
        src/prefabs/collectibles/GoldNugget.cpp
        src/prefabs/collectibles/GoldChunk.cpp
        src/prefabs/collectibles/SmallGem.cpp
        src/prefabs/items/Jetpack.cpp
        src/prefabs/items/BombSpawner.cpp
        src/prefabs/items/Bomb.cpp
        src/prefabs/items/Pistol.cpp
        src/prefabs/items/Shotgun.cpp
        src/prefabs/items/RopeSpawner.cpp
        src/prefabs/items/Rope.cpp
        src/prefabs/items/RopeChainElement.cpp
        src/prefabs/items/Whip.cpp
        src/prefabs/items/Crate.cpp
        src/prefabs/items/Cape.cpp
        src/prefabs/effects/Explosion.cpp
        src/prefabs/effects/ShotgunBlast.cpp
        src/prefabs/other/Bullet.cpp

        # Systems:

        include/system/System.hpp
        include/system/RenderingSystem.hpp
        include/system/PhysicsSystem.hpp
        include/system/ScriptingSystem.hpp
        include/system/AnimationSystem.hpp
        include/system/ParticleSystem.hpp
        include/system/CollectibleSystem.hpp
        include/system/InputSystem.hpp
        include/system/DisposingSystem.hpp
        include/system/ItemSystem.hpp

        src/system/ParticleSystem.cpp
        src/system/RenderingSystem.cpp
        src/system/PhysicsSystem.cpp
        src/system/CollectibleSystem.cpp
        src/system/AnimationSystem.cpp
        src/system/ScriptingSystem.cpp
        src/system/InputSystem.cpp
        src/system/DisposingSystem.cpp
        src/system/ItemSystem.cpp

        # Other:

        src/populator/Populator.cpp
        include/populator/Populator.hpp
        include/populator/Spawner.hpp

        src/other/Inventory.cpp
        interface/other/PhysicsComponentType.hpp
        interface/other/Inventory.hpp
        interface/other/InventoryEvent.hpp
        include/other/LootCollectedEvent.hpp
)

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
