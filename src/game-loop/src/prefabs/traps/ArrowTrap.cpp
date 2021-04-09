#include "prefabs/traps/ArrowTrap.hpp"
#include "prefabs/items/Arrow.hpp"

#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/specialized/MainDudeComponent.hpp"

#include "spritesheet-frames/NPCSpritesheetFrames.hpp"
#include "EntityRegistry.hpp"
#include "audio/Audio.hpp"

#include <cmath>

namespace
{
    constexpr float activation_distance_x = 7;
    constexpr float activation_distance_y = 0.5f;

    class ArrowTrapScript final : public ScriptBase
    {
    public:
        explicit ArrowTrapScript(HorizontalOrientation orientation) : _orientation(orientation) {}

        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            auto& registry = EntityRegistry::instance().get_registry();

            const auto& arrow_trap_position = registry.get<PositionComponent>(owner);
            const auto& arrow_trap_physics = registry.get<PhysicsComponent>(owner);
            const auto bodies = registry.view<PhysicsComponent, PositionComponent>();
            // TODO: Trap triggering should also take into consideration map tiles that
            //       may obscure bodies.

            bool triggered = false;

            bodies.each([&](PhysicsComponent& other_physics, PositionComponent& other_position)
            {
                if (triggered)
                {
                    return;
                }

                if (other_physics.get_y_velocity() == 0.0f && other_physics.get_x_velocity() == 0.0f)
                {
                    return;
                }

                switch (_orientation)
                {
                    case HorizontalOrientation::LEFT:
                    {
                        if (other_position.x_center > arrow_trap_position.x_center)
                        {
                            return;
                        }
                        break;
                    }
                    case HorizontalOrientation::RIGHT:
                    {
                        if (other_position.x_center < arrow_trap_position.x_center)
                        {
                            return;
                        }
                        break;
                    }
                }

                float distance_x = std::fabs(other_position.x_center - arrow_trap_position.x_center);
                float distance_y = std::fabs(other_position.y_center - arrow_trap_position.y_center);

                if (distance_x < activation_distance_x && distance_y < activation_distance_y)
                {
                    Audio::instance().play(SFXType::ARROW_TRAP);
                    triggered = true;

                    auto arrow = prefabs::Arrow::create(arrow_trap_position.x_center, arrow_trap_position.y_center, owner);
                    auto& arrow_physics = registry.get<PhysicsComponent>(arrow);
                    auto& arrow_position = registry.get<PositionComponent>(arrow);
                    switch (_orientation)
                    {
                        case HorizontalOrientation::LEFT:
                        {
                            arrow_physics.set_x_velocity(-0.25f);
                            arrow_position.x_center -= 0.75f;
                            break;
                        }
                        case HorizontalOrientation::RIGHT:
                        {
                            arrow_physics.set_x_velocity(+0.25f);
                            arrow_position.x_center += 0.75f;
                            break;
                        }
                    }

                    registry.destroy(owner);
                }
            });
        }

    private:
        const HorizontalOrientation _orientation;
    };
}

entt::entity prefabs::ArrowTrap::create(float pos_x_center, float pos_y_center, HorizontalOrientation orientation)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    PhysicsComponent physics(1.0f, 1.0f);
    physics.disable_gravity();

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<ScriptingComponent>(entity, std::make_shared<ArrowTrapScript>(orientation));
    // TODO: HitpointComponent + TakeExplosionDamageComponent

    return entity;
}
