#include "prefabs/npc/DamselStates.hpp"
#include "prefabs/npc/Damsel.hpp"
#include "prefabs/npc/DamselScript.hpp"
#include "prefabs/particles/HelpParticle.hpp"
#include "prefabs/particles/SmoochingParticle.hpp"

#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"
#include "components/generic/ItemComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/damage/GiveProjectileDamageComponent.hpp"
#include "components/damage/TakeMeleeDamageComponent.hpp"
#include "components/damage/TakeExplosionDamageComponent.hpp"
#include "components/damage/TakeProjectileDamageComponent.hpp"

#include "audio/Audio.hpp"
#include "Level.hpp"
#include "spritesheet-frames/NPCSpritesheetFrames.hpp"

namespace prefabs
{
    // Standing state:

    DamselBaseState* DamselStandingState::update(DamselScript& damsel, uint32_t delta_time_ms, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& physics = registry.get<PhysicsComponent>(id);
        auto& item = registry.get<ItemComponent>(id);

        if (!damsel._damsel_rescued)
        {
            _time_since_last_yell_ms += delta_time_ms;
        }

        if (_time_since_last_yell_ms > 4000)
        {
            _time_since_last_yell_ms = 0;
            return &damsel._states.yelling;
        }
        else if (item.is_carried())
        {
            return &damsel._states.held;
        }
        else if (damsel.get_panic())
        {
            return &damsel._states.running;
        }
        else if (physics.get_x_velocity() == 0.0f)
        {
            return &damsel._states.standing;
        }
        else
        {
            return &damsel._states.running;
        }
    }

    void DamselStandingState::enter(DamselScript&, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);

        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::DAMSEL_STANDING);
        animation.stop();
    }

    void DamselStandingState::exit(DamselScript&, entt::entity id)
    {
    }

    // Running state:

    DamselBaseState* DamselRunningState::update(DamselScript& damsel, uint32_t delta_time_ms, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& physics = registry.get<PhysicsComponent>(id);
        auto& item = registry.get<ItemComponent>(id);
        auto& position = registry.get<PositionComponent>(id);
        auto& horizontal_orientation = registry.get<HorizontalOrientationComponent>(id);

        if (damsel.get_panic())
        {
            if (physics.is_right_collision())
            {
                horizontal_orientation.orientation = HorizontalOrientation::LEFT;
            }

            if (physics.is_left_collision())
            {
                horizontal_orientation.orientation = HorizontalOrientation::RIGHT;
            }

            switch (horizontal_orientation.orientation)
            {
                case HorizontalOrientation::LEFT: physics.set_x_velocity(-0.05f); break;
                case HorizontalOrientation::RIGHT: physics.set_x_velocity(0.05f); break;
                default: assert(false);
            }
        }

        auto& tile_batch = Level::instance().get_tile_batch();
        MapTile* exit = nullptr;
        tile_batch.get_first_tile_of_given_type(MapTileType::EXIT, exit);
        assert(exit);

        // TODO: This should be a helper method inside the MapTile:
        ZoneComponent tile_zone(MapTile::PHYSICAL_WIDTH, MapTile::PHYSICAL_HEIGHT);
        PositionComponent tile_position(exit->get_center_x(), exit->get_center_y());

        if (physics.is_collision(tile_zone, tile_position, position))
        {
            position.x_center = exit->get_center_x();
            position.y_center = exit->get_center_y();

            return &damsel._states.exiting;
        }
        else if (item.is_carried())
        {
            return &damsel._states.held;
        }
        else if (physics.get_x_velocity() == 0.0f && physics.get_y_velocity() == 0.0f)
        {
            return &damsel._states.standing;
        }
        else
        {
            return &damsel._states.running;
        }
    }

    void DamselRunningState::enter(DamselScript&, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);

        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::DAMSEL_RUN_0_START);
        animation.start(static_cast<int>(NPCSpritesheetFrames::DAMSEL_RUN_0_START),
                        static_cast<int>(NPCSpritesheetFrames::DAMSEL_RUN_3_LAST),
                        100, true);
    }

    void DamselRunningState::exit(DamselScript&, entt::entity id)
    {
    }

    // Held state:

    DamselBaseState* DamselHeldState::update(DamselScript& damsel, uint32_t delta_time_ms, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& physics = registry.get<PhysicsComponent>(id);
        auto& position = registry.get<PositionComponent>(id);
        auto& item = registry.get<ItemComponent>(id);
        auto& hitpoints = registry.get<HitpointComponent>(id);

        auto& tile_batch = Level::instance().get_tile_batch();
        MapTile* exit = nullptr;
        tile_batch.get_first_tile_of_given_type(MapTileType::EXIT, exit);
        assert(exit);

        // TODO: This should be a helper method inside the MapTile:
        ZoneComponent tile_zone(MapTile::PHYSICAL_WIDTH, MapTile::PHYSICAL_HEIGHT);
        PositionComponent tile_position(exit->get_center_x(), exit->get_center_y());

        if (hitpoints.get_hitpoints() > 0 && physics.is_collision(tile_zone, tile_position, position))
        {
            position.x_center = exit->get_center_x();
            position.y_center = exit->get_center_y();

            return &damsel._states.exiting;
        }

        if (item.is_carried())
        {
            return this;
        }

        if (hitpoints.get_hitpoints() <= 0)
        {
            return &damsel._states.dead;
        }

        damsel.set_panic(true);
        return &damsel._states.stunned;
    }

    void DamselHeldState::enter(DamselScript&, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);

        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::DAMSEL_HELD);
        animation.stop();
    }

    void DamselHeldState::exit(DamselScript&, entt::entity id)
    {
    }
    
    // Exiting state:

    DamselBaseState* DamselExitingState::update(DamselScript& damsel, uint32_t delta_time_ms, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& animation = registry.get<AnimationComponent>(id);

        if (animation.is_finished() && !_hidden)
        {
            registry.remove<QuadComponent>(id);
            registry.remove<MeshComponent>(id);
            _hidden = true;
        }

        return this;
    }

    void DamselExitingState::enter(DamselScript& damsel, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);
        auto& physics = registry.get<PhysicsComponent>(id);
        auto& item = registry.get<ItemComponent>(id);

        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::DAMSEL_EXITING_0_START);
        animation.start(static_cast<int>(NPCSpritesheetFrames::DAMSEL_EXITING_0_START),
                        static_cast<int>(NPCSpritesheetFrames::DAMSEL_EXITING_15_LAST),
                        100, false);


        physics.disable_gravity();
        physics.set_y_velocity(0.0f);
        physics.set_x_velocity(0.0f);

        if (item.is_carried())
        {
            auto& item_carrier = item.get_item_carrier();
            item_carrier.put_down_active_item();
        }

        if (registry.has<TakeMeleeDamageComponent>(id)) registry.remove<TakeMeleeDamageComponent>(id);
        if (registry.has<TakeExplosionDamageComponent>(id)) registry.remove<TakeExplosionDamageComponent>(id);
        if (registry.has<TakeProjectileDamageComponent>(id)) registry.remove<TakeProjectileDamageComponent>(id);

        Audio::instance().play(SFXType::MAIN_DUDE_ENTERING_DOOR);

        damsel._damsel_rescued = true;
    }

    void DamselExitingState::exit(DamselScript&, entt::entity id)
    {
    }
    
    // Stunned state:

    DamselBaseState* DamselStunnedState::update(DamselScript& damsel, uint32_t delta_time_ms, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& animation = registry.get<AnimationComponent>(id);
        auto& item = registry.get<ItemComponent>(id);

        if (item.is_carried())
        {
            return &damsel._states.held;
        }
        else if (_damsel_stunned_timer_ms > 0)
        {
            _damsel_stunned_timer_ms -= delta_time_ms;
        }
        else
        {
            return &damsel._states.standing;
        }

        return this;
    }

    void DamselStunnedState::enter(DamselScript&, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);

        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::DAMSEL_STUNNED_0_START);
        animation.start(static_cast<int>(NPCSpritesheetFrames::DAMSEL_STUNNED_0_START),
                        static_cast<int>(NPCSpritesheetFrames::DAMSEL_STUNNED_4_LAST),
                        100, true);

        _damsel_stunned_timer_ms = 5000;
    }

    void DamselStunnedState::exit(DamselScript&, entt::entity id)
    {
        _damsel_stunned_timer_ms = 0;
    }
    
    // Yelling state:

    DamselBaseState* DamselYellingState::update(DamselScript& damsel, uint32_t delta_time_ms, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& animation = registry.get<AnimationComponent>(id);
        auto& item = registry.get<ItemComponent>(id);

        if (item.is_carried())
        {
            return &damsel._states.held;
        }
        else if (animation.is_finished())
        {
            return &damsel._states.standing;
        }

        return this;
    }

    void DamselYellingState::enter(DamselScript&, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& position = registry.get<PositionComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);

        prefabs::HelpParticle::create(position.x_center, position.y_center - MapTile::PHYSICAL_HEIGHT);

        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::DAMSEL_YELL_0_START);
        animation.start(static_cast<int>(NPCSpritesheetFrames::DAMSEL_YELL_0_START),
                        static_cast<int>(NPCSpritesheetFrames::DAMSEL_YELL_9_LAST),
                        100, false);
    }

    void DamselYellingState::exit(DamselScript&, entt::entity id)
    {
    }
    
    // Dead state:

    DamselBaseState* DamselDeadState::update(DamselScript& damsel, uint32_t delta_time_ms, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& item = registry.get<ItemComponent>(id);
        auto& physics = registry.get<PhysicsComponent>(id);

        if (item.is_carried())
        {
            return &damsel._states.held;
        }
        else if (physics.get_y_velocity() < 0.0f)
        {
            return &damsel._states.bouncing;
        }
        else if (physics.get_y_velocity() > 0.0f)
        {
            return &damsel._states.falling;
        }

        return this;
    }

    void DamselDeadState::enter(DamselScript&, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);
        auto& physics = registry.get<PhysicsComponent>(id);

        physics.set_bounciness(0.4f);
        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::DAMSEL_DEAD);
        animation.stop();

        if (!registry.has<GiveProjectileDamageComponent>(id)) registry.emplace<GiveProjectileDamageComponent>(id, 1);
    }

    void DamselDeadState::exit(DamselScript&, entt::entity id)
    {
    }

    // Falling state:

    DamselBaseState* DamselFallingState::update(DamselScript& damsel, uint32_t delta_time_ms, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& item = registry.get<ItemComponent>(id);
        auto& physics = registry.get<PhysicsComponent>(id);

        if (item.is_carried())
        {
            return &damsel._states.held;
        }
        else if (physics.get_y_velocity() < 0.0f)
        {
            return &damsel._states.bouncing;
        }
        else if (physics.get_y_velocity() == 0.0f)
        {
            return &damsel._states.dead;
        }

        return this;
    }

    void DamselFallingState::enter(DamselScript&, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);

        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::DAMSEL_FALL);
        animation.stop();
    }

    void DamselFallingState::exit(DamselScript&, entt::entity id)
    {
    }

    // Bouncing state:

    DamselBaseState* DamselBouncingState::update(DamselScript& damsel, uint32_t delta_time_ms, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& item = registry.get<ItemComponent>(id);
        auto& physics = registry.get<PhysicsComponent>(id);

        if (item.is_carried())
        {
            return &damsel._states.held;
        }
        else if (physics.get_y_velocity() > 0.0f)
        {
            return &damsel._states.falling;
        }
        else if (physics.get_y_velocity() == 0.0f)
        {
            return &damsel._states.dead;
        }

        return this;
    }

    void DamselBouncingState::enter(DamselScript&, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);

        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::DAMSEL_BOUNCE);
        animation.stop();
    }

    void DamselBouncingState::exit(DamselScript&, entt::entity id)
    {
    }

    // Smooching state:

    DamselBaseState* DamselSmoochingState::update(DamselScript& damsel, uint32_t delta_time_ms, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& animation = registry.get<AnimationComponent>(id);

        if (animation.is_finished())
        {
            return &damsel._states.standing;
        }

        return this;
    }

    void DamselSmoochingState::enter(DamselScript&, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);
        auto& position = registry.get<PositionComponent>(id);

        prefabs::SmoochingParticle::create(position.x_center, position.y_center);

        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::DAMSEL_KISS_0_START);
        animation.start(static_cast<int>(NPCSpritesheetFrames::DAMSEL_KISS_0_START),
                        static_cast<int>(NPCSpritesheetFrames::DAMSEL_KISS_9_LAST),
                        100, false);

        Audio::instance().play(SFXType::KISS);
    }

    void DamselSmoochingState::exit(DamselScript&, entt::entity id)
    {
    }
}
