#include "prefabs/npc/ShopkeeperStates.hpp"
#include "prefabs/npc/Shopkeeper.hpp"
#include "prefabs/npc/ShopkeeperScript.hpp"

#include "components/generic/QuadComponent.hpp"
#include "components/generic/ItemComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/damage/TakeJumpOnTopDamage.hpp"
#include "spritesheet-frames/NPCSpritesheetFrames.hpp"

namespace prefabs
{
    // Standing state:
    
    ShopkeeperBaseState* ShopkeeperStandingState::update(ShopkeeperScript& shopkeeper, uint32_t delta_time_ms, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& physics = registry.get<PhysicsComponent>(id);

        if (physics.get_x_velocity() == 0.0f)
        {
            return &shopkeeper._states.standing;
        }
        else
        {
            return &shopkeeper._states.running;
        }
    }

    void ShopkeeperStandingState::enter(ShopkeeperScript&, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);

        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::SHOPKEEPER_STANDING);
        animation.stop();
    }

    void ShopkeeperStandingState::exit(ShopkeeperScript&, entt::entity id)
    {
    }
    
    // Running state:
    
    ShopkeeperBaseState* ShopkeeperRunningState::update(ShopkeeperScript& shopkeeper, uint32_t delta_time_ms, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& physics = registry.get<PhysicsComponent>(id);

        if (physics.get_x_velocity() == 0.0f && physics.get_y_velocity() == 0.0f)
        {
            return &shopkeeper._states.standing;
        }
        else
        {
            return &shopkeeper._states.running;
        }
    }

    void ShopkeeperRunningState::enter(ShopkeeperScript&, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);

        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::SHOPKEEPER_RUN_0_START);
        animation.start(static_cast<int>(NPCSpritesheetFrames::SHOPKEEPER_RUN_0_START),
                        static_cast<int>(NPCSpritesheetFrames::SHOPKEEPER_RUN_5_LAST),
                        100, true);
    }

    void ShopkeeperRunningState::exit(ShopkeeperScript&, entt::entity id)
    {
    }
    
    // Stunned state:
    
    ShopkeeperBaseState* ShopkeeperStunnedState::update(ShopkeeperScript& shopkeeper, uint32_t delta_time_ms, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& physics = registry.get<PhysicsComponent>(id);

        _shopkeeper_stunned_timer_ms -= delta_time_ms;
        if (_shopkeeper_stunned_timer_ms <= 0)
        {
            _shopkeeper_stunned_timer_ms = 0;

            if (physics.get_x_velocity() == 0.0f && physics.get_y_velocity() == 0.0f)
            {
                return &shopkeeper._states.standing;
            }
            else
            {
                return &shopkeeper._states.running;
            }
        }
        else
        {
            return &shopkeeper._states.stunned;
        }
    }

    void ShopkeeperStunnedState::enter(ShopkeeperScript&, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);

        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::SHOPKEEPER_STUNNED_0_START);
        animation.start(static_cast<int>(NPCSpritesheetFrames::SHOPKEEPER_STUNNED_0_START),
                        static_cast<int>(NPCSpritesheetFrames::SHOPKEEPER_STUNNED_5_LAST),
                        100, true);

        _shopkeeper_stunned_timer_ms = 2000;
    }


    void ShopkeeperStunnedState::exit(ShopkeeperScript&, entt::entity id)
    {
    }
    
    // Dead state:
    
    ShopkeeperBaseState* ShopkeeperDeadState::update(ShopkeeperScript& shopkeeper, uint32_t delta_time_ms, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& item = registry.get<ItemComponent>(id);
        auto& physics = registry.get<PhysicsComponent>(id);

        if (item.is_carried())
        {
            return &shopkeeper._states.held_dead;
        }
        else if (physics.get_y_velocity() < 0.0f)
        {
            return &shopkeeper._states.bouncing;
        }
        else if (physics.get_y_velocity() > 0.0f)
        {
            return &shopkeeper._states.falling;
        }

        return this;
    }

    void ShopkeeperDeadState::enter(ShopkeeperScript&, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);
        auto& physics = registry.get<PhysicsComponent>(id);

        physics.set_bounciness(0.4f);
        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::SHOPKEEPER_DEAD);
        animation.stop();

        if (registry.has<TakeJumpOnTopDamageComponent>(id)) registry.remove<TakeJumpOnTopDamageComponent>(id);
    }

    void ShopkeeperDeadState::exit(ShopkeeperScript&, entt::entity id)
    {
    }
    
    // Held dead state:
    
    ShopkeeperBaseState* ShopkeeperHeldDeadState::update(ShopkeeperScript& shopkeeper, uint32_t delta_time_ms, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& physics = registry.get<PhysicsComponent>(id);
        auto& item = registry.get<ItemComponent>(id);

        if (!item.is_carried())
        {
            return &shopkeeper._states.dead;
        }

        return this;
    }

    void ShopkeeperHeldDeadState::enter(ShopkeeperScript&, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);

        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::SHOPKEEPER_DEAD_HELD);
        animation.stop();
    }


    void ShopkeeperHeldDeadState::exit(ShopkeeperScript&, entt::entity id)
    {
    }
    
    // Held falling:
    
    ShopkeeperBaseState* ShopkeeperFallingState::update(ShopkeeperScript& shopkeeper, uint32_t delta_time_ms, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& item = registry.get<ItemComponent>(id);
        auto& physics = registry.get<PhysicsComponent>(id);

        if (item.is_carried())
        {
            return &shopkeeper._states.held_dead;
        }
        else if (physics.get_y_velocity() < 0.0f)
        {
            return &shopkeeper._states.bouncing;
        }
        else if (physics.get_y_velocity() == 0.0f)
        {
            return &shopkeeper._states.dead;
        }

        return this;
    }

    void ShopkeeperFallingState::enter(ShopkeeperScript&, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);

        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::SHOPKEEPER_FALLING);
        animation.stop();
    }

    void ShopkeeperFallingState::exit(ShopkeeperScript&, entt::entity id)
    {
    }

    // Held bouncing:
    
    ShopkeeperBaseState* ShopkeeperBouncingState::update(ShopkeeperScript& shopkeeper, uint32_t delta_time_ms, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& physics = registry.get<PhysicsComponent>(id);
        auto& item = registry.get<ItemComponent>(id);

        if (item.is_carried())
        {
            return &shopkeeper._states.held_dead;
        }
        else if (physics.get_y_velocity() > 0.0f)
        {
            return &shopkeeper._states.falling;
        }
        else if (physics.get_y_velocity() == 0.0f)
        {
            return &shopkeeper._states.dead;
        }

        return this;
    }

    void ShopkeeperBouncingState::enter(ShopkeeperScript&, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);

        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::SHOPKEEPER_BOUNCING);
        animation.stop();
    }

    void ShopkeeperBouncingState::exit(ShopkeeperScript&, entt::entity id)
    {
    }
}
