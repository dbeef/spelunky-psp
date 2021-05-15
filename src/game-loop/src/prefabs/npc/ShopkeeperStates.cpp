#include "prefabs/npc/ShopkeeperStates.hpp"
#include "prefabs/npc/Shopkeeper.hpp"
#include "prefabs/npc/ShopkeeperScript.hpp"

#include "components/generic/QuadComponent.hpp"
#include "components/generic/SaleableComponent.hpp"
#include "components/generic/ItemComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/ActivableComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/damage/TakeJumpOnTopDamage.hpp"
#include "components/damage/GiveNpcTouchDamageComponent.hpp"
#include "components/damage/GiveProjectileDamageComponent.hpp"

#include "spritesheet-frames/NPCSpritesheetFrames.hpp"

namespace prefabs
{
    // Standing state:
    
    ShopkeeperBaseState* ShopkeeperStandingState::update(ShopkeeperScript& shopkeeper, uint32_t delta_time_ms, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& physics = registry.get<PhysicsComponent>(id);

        shopkeeper.follow_customer(id);
        shopkeeper.do_angry_stuff(id, delta_time_ms);

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

        shopkeeper.follow_customer(id);
        shopkeeper.do_angry_stuff(id, delta_time_ms);

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
        auto& stunned_timer_ms = shopkeeper._stunned_timer_ms;
        auto& registry = EntityRegistry::instance().get_registry();
        auto& physics = registry.get<PhysicsComponent>(id);
        auto& item = registry.get<ItemComponent>(id);

        stunned_timer_ms -= delta_time_ms;
        if (stunned_timer_ms <= 0)
        {
            stunned_timer_ms = 0;

            if (physics.get_x_velocity() == 0.0f && physics.get_y_velocity() == 0.0f)
            {
                return &shopkeeper._states.standing;
            }
            else
            {
                return &shopkeeper._states.running;
            }
        }
        else if (item.is_carried())
        {
            return &shopkeeper._states.held_stunned;
        }

        return this;
    }

    void ShopkeeperStunnedState::enter(ShopkeeperScript& shopkeeper, entt::entity id)
    {
        auto& stunned_timer_ms = shopkeeper._stunned_timer_ms;
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);
        auto& item_carrier = registry.get<ItemCarrierComponent>(id);

        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::SHOPKEEPER_STUNNED_0_START);
        animation.start(static_cast<int>(NPCSpritesheetFrames::SHOPKEEPER_STUNNED_0_START),
                        static_cast<int>(NPCSpritesheetFrames::SHOPKEEPER_STUNNED_5_LAST),
                        100, true);
        item_carrier.put_down_active_item();

        stunned_timer_ms = 2000;

        if (registry.has<TakeJumpOnTopDamageComponent>(id)) registry.remove<TakeJumpOnTopDamageComponent>(id);
    }


    void ShopkeeperStunnedState::exit(ShopkeeperScript&, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& hitpoint = registry.get<HitpointComponent>(id);
        if (hitpoint.get_hitpoints() > 0)
        {
            if (!registry.has<TakeJumpOnTopDamageComponent>(id)) registry.emplace<TakeJumpOnTopDamageComponent>(id);
        }
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

    void ShopkeeperDeadState::enter(ShopkeeperScript& shopkeeper, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);
        auto& physics = registry.get<PhysicsComponent>(id);
        auto& item_carrier = registry.get<ItemCarrierComponent>(id);

        physics.set_bounciness(0.4f);
        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::SHOPKEEPER_DEAD);
        animation.stop();
        item_carrier.put_down_active_item();

        if (registry.has<TakeJumpOnTopDamageComponent>(id)) registry.remove<TakeJumpOnTopDamageComponent>(id);
        if (registry.has<GiveNpcTouchDamageComponent>(id)) registry.remove<GiveNpcTouchDamageComponent>(id);
        if (!registry.has<GiveProjectileDamageComponent>(id)) registry.emplace<GiveProjectileDamageComponent>(id, 1);
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
    
    // Held stunned state:
    
    ShopkeeperBaseState* ShopkeeperHeldStunnedState::update(ShopkeeperScript& shopkeeper, uint32_t delta_time_ms, entt::entity id)
    {
        auto& stunned_timer_ms = shopkeeper._stunned_timer_ms;
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

            return &shopkeeper._states.running;
        }
        else if (!item.is_carried())
        {
            return &shopkeeper._states.stunned;
        }

        return this;
    }

    void ShopkeeperHeldStunnedState::enter(ShopkeeperScript&, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);
        auto& item_carrier = registry.get<ItemCarrierComponent>(id);

        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::SHOPKEEPER_HELD_STUNNED_0_START);
        animation.start(static_cast<int>(NPCSpritesheetFrames::SHOPKEEPER_HELD_STUNNED_0_START),
                        static_cast<int>(NPCSpritesheetFrames::SHOPKEEPER_HELD_STUNNED_5_LAST),
                        100, true);
        item_carrier.put_down_active_item();

        if (registry.has<TakeJumpOnTopDamageComponent>(id)) registry.remove<TakeJumpOnTopDamageComponent>(id);
    }

    void ShopkeeperHeldStunnedState::exit(ShopkeeperScript&, entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& hitpoint = registry.get<HitpointComponent>(id);
        if (hitpoint.get_hitpoints() > 0)
        {
            if (!registry.has<TakeJumpOnTopDamageComponent>(id)) registry.emplace<TakeJumpOnTopDamageComponent>(id);
        }
    }
    
    // Falling:
    
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

    // Bouncing:
    
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
