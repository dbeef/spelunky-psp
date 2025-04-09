#include "prefabs/npc/CavemanStates.hpp"
#include "prefabs/npc/CavemanScript.hpp"

#include "components/generic/QuadComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"
#include "components/generic/ItemComponent.hpp"
#include "components/damage/TakeJumpOnTopDamage.hpp"
#include "components/damage/TakeMeleeDamageComponent.hpp"
#include "components/damage/GiveNpcTouchDamageComponent.hpp"
#include "components/damage/GiveProjectileDamageComponent.hpp"

#include "spritesheet-frames/NPCSpritesheetFrames.hpp"
#include "EntityRegistry.hpp"

namespace
{
    constexpr uint16_t max_running_time_ms = 5000;
    constexpr uint16_t max_standing_state_ms = 3000;
    constexpr uint16_t stun_time_ms = 5000;
}

namespace prefabs
{
    // Standing state:
    
    CavemanBaseState* CavemanStandingState::update(CavemanScript& caveman, uint32_t delta_time_ms, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& physics = registry.get<PhysicsComponent>(id);

        if (caveman._angry)
        {
            return &caveman._states.running;
        }

        _standing_timer_ms -= delta_time_ms;
        if (_standing_timer_ms <= 0)
        {
            auto& orientation = registry.get<HorizontalOrientationComponent>(id);
            orientation.orientation = static_cast<HorizontalOrientation>(std::rand() % 2);
            caveman._states.running.set_random_running_timer();
            return &caveman._states.running;
        }

        if (physics.get_x_velocity() == 0.0f)
        {
            return this;
        }
        else
        {
            return &caveman._states.running;
        }
    }

    void CavemanStandingState::enter(CavemanScript& caveman, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);

        animation.stop();
        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::CAVEMAN);
    }

    void CavemanStandingState::exit(CavemanScript& caveman, entt::entity id)
    {
    }

    void CavemanStandingState::set_random_standing_timer()
    {
        _standing_timer_ms = std::rand() % max_standing_state_ms;
    }

    // Running state:
    
    CavemanBaseState* CavemanRunningState::update(CavemanScript& caveman, uint32_t delta_time_ms, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& physics = registry.get<PhysicsComponent>(id);
        auto& orientation = registry.get<HorizontalOrientationComponent>(id);

        _running_timer_ms -= delta_time_ms;
        if (_running_timer_ms <= 0 && !caveman._angry)
        {
            caveman._states.standing.set_random_standing_timer();
            return &caveman._states.standing;
        }

        if (physics.is_right_collision())
        {
            orientation.orientation = HorizontalOrientation::LEFT;
        }

        if (physics.is_left_collision())
        {
            orientation.orientation = HorizontalOrientation::RIGHT;
        }

        const float running_speed = caveman._angry ? 0.065f : 0.01f;

        switch (orientation.orientation)
        {
            case HorizontalOrientation::LEFT: physics.set_x_velocity(-running_speed); break;
            case HorizontalOrientation::RIGHT: physics.set_x_velocity(running_speed); break;
            default: assert(false);
        }

        if (physics.get_x_velocity() == 0.0f && physics.get_y_velocity() == 0.0f)
        {
            return &caveman._states.standing;
        }
        else
        {
            return this;
        }
    }

    void CavemanRunningState::enter(CavemanScript& caveman, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);

        animation.start(static_cast<int>(NPCSpritesheetFrames::CAVEMAN_RUN_0_FIRST),
                        static_cast<int>(NPCSpritesheetFrames::CAVEMAN_RUN_3_LAST),
                        caveman._angry ? 65 : 100, true);
        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::CAVEMAN_RUN_0_FIRST);
    }

    void CavemanRunningState::exit(CavemanScript& caveman, entt::entity id)
    {
    }

    void CavemanRunningState::set_random_running_timer()
    {
        _running_timer_ms = std::rand() % max_running_time_ms;
    }

    // Dead state:
    
    CavemanBaseState* CavemanDeadState::update(CavemanScript& caveman, uint32_t delta_time_ms, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& item = registry.get<ItemComponent>(id);
        auto& physics = registry.get<PhysicsComponent>(id);

        if (item.is_carried())
        {
            return &caveman._states.held_dead;
        }
        else if (physics.get_y_velocity() < 0.0f)
        {
            return &caveman._states.bouncing;
        }
        else if (physics.get_y_velocity() > 0.0f)
        {
            return &caveman._states.falling;
        }

        return this;
    }

    void CavemanDeadState::enter(CavemanScript& caveman, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);
        auto& physics = registry.get<PhysicsComponent>(id);

        animation.stop();
        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::CAVEMAN_DEAD);
        physics.set_bounciness(0.4f);

        if (registry.has<TakeMeleeDamageComponent>(id)) registry.remove<TakeMeleeDamageComponent>(id);
        if (registry.has<TakeJumpOnTopDamageComponent>(id)) registry.remove<TakeJumpOnTopDamageComponent>(id);
        if (registry.has<GiveNpcTouchDamageComponent>(id)) registry.remove<GiveNpcTouchDamageComponent>(id);
        if (!registry.has<GiveProjectileDamageComponent>(id)) registry.emplace<GiveProjectileDamageComponent>(id, 1);
    }

    void CavemanDeadState::exit(CavemanScript& caveman, entt::entity id)
    {
    }

    // Held dead state:
    
    CavemanBaseState* CavemanHeldDeadState::update(CavemanScript& caveman, uint32_t delta_time_ms, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& item = registry.get<ItemComponent>(id);

        if (item.is_carried())
        {
            return this;
        }
        else
        {
            return &caveman._states.dead;
        }
    }

    void CavemanHeldDeadState::enter(CavemanScript& caveman, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);

        animation.stop();
        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::CAVEMAN_HELD);
    }

    void CavemanHeldDeadState::exit(CavemanScript& caveman, entt::entity id)
    {
    }

    // Falling state:
    
    CavemanBaseState* CavemanFallingState::update(CavemanScript& caveman, uint32_t delta_time_ms, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& item = registry.get<ItemComponent>(id);
        auto& physics = registry.get<PhysicsComponent>(id);

        if (item.is_carried())
        {
            return &caveman._states.held_dead;
        }
        else if (physics.get_y_velocity() < 0.0f)
        {
            return &caveman._states.bouncing;
        }
        else if (physics.get_y_velocity() == 0.0f)
        {
            return &caveman._states.dead;
        }

        return this;
    }

    void CavemanFallingState::enter(CavemanScript& caveman, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);

        animation.stop();
        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::CAVEMAN_FALL);
    }

    void CavemanFallingState::exit(CavemanScript& caveman, entt::entity id)
    {
    }

    // Bouncing state:
    
    CavemanBaseState* CavemanBouncingState::update(CavemanScript& caveman, uint32_t delta_time_ms, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& item = registry.get<ItemComponent>(id);
        auto& physics = registry.get<PhysicsComponent>(id);

        if (item.is_carried())
        {
            return &caveman._states.held_dead;
        }
        else if (physics.get_y_velocity() > 0.0f)
        {
            return &caveman._states.falling;
        }
        else if (physics.get_y_velocity() == 0.0f)
        {
            return &caveman._states.dead;
        }

        return this;
    }

    void CavemanBouncingState::enter(CavemanScript& caveman, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);

        animation.stop();
        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::CAVEMAN_BOUNCE);
    }

    void CavemanBouncingState::exit(CavemanScript& caveman, entt::entity id)
    {
    }

    // Stunned state:
    
    CavemanBaseState* CavemanStunnedState::update(CavemanScript& caveman, uint32_t delta_time_ms, entt::entity id)
    {
        auto& stunned_timer_ms = caveman._stunned_timer_ms;
        auto& registry = EntityRegistry::instance().get_registry();
        auto& physics = registry.get<PhysicsComponent>(id);
        auto& item = registry.get<ItemComponent>(id);

        stunned_timer_ms -= delta_time_ms;
        if (stunned_timer_ms <= 0)
        {
            stunned_timer_ms = 0;

            if (physics.get_x_velocity() == 0.0f && physics.get_y_velocity() == 0.0f)
            {
                return &caveman._states.standing;
            }
            else
            {
                return &caveman._states.running;
            }
        }
        else if (item.is_carried())
        {
            return &caveman._states.held_stunned;
        }

        return this;
    }

    void CavemanStunnedState::enter(CavemanScript& caveman, entt::entity id)
    {
        auto& stunned_timer_ms = caveman._stunned_timer_ms;
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);

        animation.start(static_cast<int>(NPCSpritesheetFrames::CAVEMAN_STUNNED_0_FIRST),
                        static_cast<int>(NPCSpritesheetFrames::CAVEMAN_STUNNED_4_LAST),
                        100, true);
        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::CAVEMAN_STUNNED_0_FIRST);

        stunned_timer_ms = stun_time_ms;

        if (registry.has<TakeJumpOnTopDamageComponent>(id)) registry.remove<TakeJumpOnTopDamageComponent>(id);
        if (registry.has<TakeMeleeDamageComponent>(id)) registry.remove<TakeMeleeDamageComponent>(id);
        if (registry.has<GiveNpcTouchDamageComponent>(id)) registry.remove<GiveNpcTouchDamageComponent>(id);
    }

    void CavemanStunnedState::exit(CavemanScript& caveman, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& hitpoint = registry.get<HitpointComponent>(id);
        if (hitpoint.get_hitpoints() > 0)
        {
            caveman.add_take_jump_on_top_damage_component(id);
            caveman.add_take_melee_damage_component(id);

            if (!registry.has<GiveNpcTouchDamageComponent>(id)) registry.emplace<GiveNpcTouchDamageComponent>(id);
        }
    }

    // Held stunned state:
    
    CavemanBaseState* CavemanHeldStunnedState::update(CavemanScript& caveman, uint32_t delta_time_ms, entt::entity id)
    {
        auto& stunned_timer_ms = caveman._stunned_timer_ms;
        auto& registry = EntityRegistry::instance().get_registry();
        auto& item = registry.get<ItemComponent>(id);
        auto& physics = registry.get<PhysicsComponent>(id);

        stunned_timer_ms -= delta_time_ms;

        if (stunned_timer_ms <= 0)
        {
            if (item.is_carried())
            {
                auto& item_carrier = item.get_item_carrier();
                item_carrier.put_down_active_item();
                physics.add_velocity(0.0f, -0.075f);
            }

            return &caveman._states.running;
        }
        else if (!item.is_carried())
        {
            return &caveman._states.stunned;
        }

        return this;
    }

    void CavemanHeldStunnedState::enter(CavemanScript& caveman, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);

        animation.start(static_cast<int>(NPCSpritesheetFrames::CAVEMAN_STUNNED_HELD_0_FIRST),
                        static_cast<int>(NPCSpritesheetFrames::CAVEMAN_STUNNED_HELD_4_LAST),
                        100, true);
        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::CAVEMAN_STUNNED_HELD_0_FIRST);

        if (registry.has<TakeJumpOnTopDamageComponent>(id)) registry.remove<TakeJumpOnTopDamageComponent>(id);
        if (registry.has<TakeMeleeDamageComponent>(id)) registry.remove<TakeMeleeDamageComponent>(id);
        if (registry.has<GiveNpcTouchDamageComponent>(id)) registry.remove<GiveNpcTouchDamageComponent>(id);
    }

    void CavemanHeldStunnedState::exit(CavemanScript& caveman, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& hitpoint = registry.get<HitpointComponent>(id);
        if (hitpoint.get_hitpoints() > 0)
        {
            caveman.add_take_jump_on_top_damage_component(id);
            caveman.add_take_melee_damage_component(id);

            if (!registry.has<GiveNpcTouchDamageComponent>(id)) registry.emplace<GiveNpcTouchDamageComponent>(id);
        }
    }
}
