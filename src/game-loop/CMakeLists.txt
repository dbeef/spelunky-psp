project(GameLoop)

add_library(GameLoop STATIC
        # Game loop:

        src/game-loop/GameLoop.cpp
        src/game-loop/GameLoopMainMenuState.cpp
        src/game-loop/GameLoopPlayingState.cpp
        src/game-loop/GameLoopStartedState.cpp
        src/game-loop/GameLoopLevelSummaryState.cpp
        src/game-loop/GameLoopScoresState.cpp
        src/game-loop/GameLoopSandboxState.cpp

        interface/game-loop/GameLoop.hpp
        interface/game-loop/GameLoopBaseState.hpp
        interface/game-loop/GameLoopMainMenuState.hpp
        interface/game-loop/GameLoopPlayingState.hpp
        interface/game-loop/GameLoopStartedState.hpp
        interface/game-loop/GameLoopLevelSummaryState.hpp
        interface/game-loop/GameLoopScoresState.hpp
        interface/game-loop/GameLoopSandboxState.hpp

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
        include/system/DamageSystem.hpp
        include/system/ShoppingSystem.hpp

        src/system/ShoppingSystem.cpp
        src/system/ParticleSystem.cpp
        src/system/RenderingSystem.cpp
        src/system/PhysicsSystem.cpp
        src/system/CollectibleSystem.cpp
        src/system/AnimationSystem.cpp
        src/system/ScriptingSystem.cpp
        src/system/InputSystem.cpp
        src/system/DisposingSystem.cpp
        src/system/ItemSystem.cpp
        src/system/DamageSystem.cpp

        # Other:

        src/game-loop/GameLoopState.cpp
        interface/game-loop/GameLoopState.hpp
        interface/game-loop/GameLoopEditorState.hpp
        src/game-loop/GameLoopEditorState.cpp
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
        Events
    PRIVATE
        Other
        MainDude
        Components
        Logger
        Input
        RenderingTypes
        Collisions
        Level
        Patterns
        Audio
        glad
        GraphicsUtils
        $<$<BOOL:${SPELUNKY_PSP_WITH_IMGUI}>:imgui>
        Dependencies
        cjson
        Assets
)
