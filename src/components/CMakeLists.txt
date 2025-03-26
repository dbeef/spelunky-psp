project(Components)

add_library(Components STATIC
        interface/components/damage/TakeProjectileDamageComponent.hpp
        interface/components/damage/GiveExplosionDamageComponent.hpp
        interface/components/damage/TakeSpikesDamageComponent.hpp
        interface/components/damage/GiveNpcTouchDamageComponent.hpp
        interface/components/damage/TakeNpcTouchDamageComponent.hpp
        interface/components/damage/GiveSpikesDamageComponent.hpp
        interface/components/damage/GiveMeleeDamageComponent.hpp
        interface/components/damage/TakeExplosionDamageComponent.hpp
        interface/components/damage/TakeMeleeDamageComponent.hpp
        interface/components/damage/TakeFallDamageComponent.hpp
        interface/components/damage/HitpointComponent.hpp
        interface/components/damage/TakeJumpOnTopDamage.hpp
        interface/components/damage/GiveProjectileDamageComponent.hpp
        interface/components/damage/TakeTileCollisionDamageComponent.hpp
        interface/components/damage/GiveJumpOnTopDamage.hpp
        interface/components/generic/InputComponent.hpp
        interface/components/generic/ActivableComponent.hpp
        interface/components/generic/SaleableComponent.hpp
        interface/components/generic/ZoneComponent.hpp
        interface/components/generic/PositionComponent.hpp
        interface/components/generic/ClimbingComponent.hpp
        interface/components/generic/QuadComponent.hpp
        interface/components/generic/TextComponent.hpp
        interface/components/generic/ItemCarrierComponent.hpp
        interface/components/generic/PhysicsComponent.hpp
        interface/components/generic/OpenableComponent.hpp
        interface/components/generic/AnimationComponent.hpp
        interface/components/generic/BlinkingComponent.hpp
        interface/components/generic/TimeLimitComponent.hpp
        interface/components/generic/CollectibleComponent.hpp
        interface/components/generic/MeshComponent.hpp
        interface/components/generic/ImguiComponent.hpp
        interface/components/generic/ItemComponent.hpp
        interface/components/generic/ScriptingComponent.hpp
        interface/components/generic/NpcTypeComponent.hpp
        interface/components/generic/ParticleEmitterComponent.hpp
        interface/components/generic/HorizontalOrientationComponent.hpp
        interface/components/specialized/DeathOverlayComponent.hpp
        interface/components/specialized/HudOverlayObservers.hpp
        interface/components/specialized/MainDudeComponent.hpp
        interface/components/specialized/HudOverlayComponent.hpp
        interface/components/specialized/PauseOverlayComponent.hpp
        interface/components/specialized/LevelSummaryTracker.hpp
        interface/components/specialized/LevelSummaryOverlayComponent.hpp
        src/components/generic/AnimationComponent.cpp
        src/components/generic/TextComponent.cpp
        src/components/generic/QuadComponent.cpp
        src/components/generic/SaleableComponent.cpp
        src/components/generic/PhysicsComponent.cpp
        src/components/generic/ItemComponent.cpp
        src/components/generic/ItemCarrierComponent.cpp
        src/components/specialized/LevelSummaryTracker.cpp
        src/components/specialized/LevelSummaryOverlayComponent.cpp
        src/components/specialized/DeathOverlayComponent.cpp
        src/components/specialized/HudOverlayObservers.cpp
        src/components/specialized/HudOverlayComponent.cpp
        src/components/specialized/PauseOverlayComponent.cpp
)

target_include_directories(Components
        PRIVATE interface interface
        INTERFACE interface
)

set_target_properties(Components PROPERTIES
        CXX_STANDARD
        17
)

target_link_libraries(Components
        PUBLIC
        Viewport
        Patterns
        EntityRegistry
        RenderingTypes
        Level
        MainDude
        Collisions
        PRIVATE
        Logger
        Other
)

if (SPELUNKY_PSP_PLATFORM_LINUX)
    target_link_libraries(Components PRIVATE dl)
    target_compile_definitions(Components PRIVATE SPELUNKY_PSP_PLATFORM_LINUX)
elseif (SPELUNKY_PSP_PLATFORM_PSP)
    target_compile_definitions(Components PRIVATE SPELUNKY_PSP_PLATFORM_PSP)
endif ()
