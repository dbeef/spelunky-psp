#include "prefabs/npc/ShopkeeperStates.hpp"
#include "prefabs/npc/Shopkeeper.hpp"
#include "prefabs/npc/ShopkeeperScript.hpp"

#include "components/generic/QuadComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
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

    void ShopkeeperStandingState::enter(entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);

        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::SHOPKEEPER_STANDING);
        animation.stop();
    }

    void ShopkeeperStandingState::exit(entt::entity id)
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

    void ShopkeeperRunningState::enter(entt::entity id)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& quad = registry.get<QuadComponent>(id);
        auto& animation = registry.get<AnimationComponent>(id);

        quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::SHOPKEEPER_RUN_0_START);
        animation.start(static_cast<int>(NPCSpritesheetFrames::SHOPKEEPER_RUN_0_START),
                        static_cast<int>(NPCSpritesheetFrames::SHOPKEEPER_RUN_5_LAST),
                        100, true);
    }

    void ShopkeeperRunningState::exit(entt::entity id)
    {
    }
}
