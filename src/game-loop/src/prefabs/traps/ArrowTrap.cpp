#include "prefabs/traps/ArrowTrap.hpp"
#include "prefabs/items/Arrow.hpp"

#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/damage/TakeExplosionDamageComponent.hpp"
#include "components/damage/HitpointComponent.hpp"
#include "components/specialized/MainDudeComponent.hpp"

#include "spritesheet-frames/NPCSpritesheetFrames.hpp"
#include "EntityRegistry.hpp"
#include "audio/Audio.hpp"
#include "Level.hpp"

#include <cmath>

namespace
{
    constexpr float activation_distance_x = 7;
    constexpr float activation_distance_y = 0.5f;

    class ArrowTrapExplosionObserver : public Observer<ExplosionDamageTakenEvent>
    {
    public:
        explicit ArrowTrapExplosionObserver(entt::entity owner) : _owner(owner)
        {
        }

        void on_notify(const ExplosionDamageTakenEvent* event) override
        {
            auto &registry = EntityRegistry::instance().get_registry();

            // Check if arrow-trap tile was destroyed to confirm entity can be disposed:
            auto &tile_batch = Level::instance().get_tile_batch();
            auto &position = registry.get<PositionComponent>(_owner);

            auto *arrow_trap_tile = tile_batch.map_tiles[static_cast<int>(position.x_center)][static_cast<int>(position.y_center)];

            if (arrow_trap_tile->map_tile_type != MapTileType::ARROW_TRAP_LEFT &&
                arrow_trap_tile->map_tile_type != MapTileType::ARROW_TRAP_RIGHT)
            {
                // Can be disposed:
                registry.destroy(_owner);
            }
        }
    private:
        entt::entity _owner;
    };

    class ArrowTrapScript final : public ScriptBase
    {
    public:
        explicit ArrowTrapScript(HorizontalOrientation orientation, entt::entity owner)
            : _orientation(orientation)
            , _explosion_observer(owner)
        {}

        ArrowTrapExplosionObserver* get_explosion_observer()
        {
            return &_explosion_observer;
        }

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
        ArrowTrapExplosionObserver _explosion_observer;
    };
}

entt::entity prefabs::ArrowTrap::create(float pos_x_center, float pos_y_center, HorizontalOrientation orientation)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    PhysicsComponent physics(1.0f, 1.0f);
    physics.disable_gravity();

    auto arrow_trap_script = std::make_shared<ArrowTrapScript>(orientation, entity);

    TakeExplosionDamageComponent explosion_component;
    explosion_component.add_observer(reinterpret_cast<Observer<ExplosionDamageTakenEvent>*>(arrow_trap_script->get_explosion_observer()));

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<ScriptingComponent>(entity, arrow_trap_script);
    registry.emplace<HitpointComponent>(entity, 1, false);
    registry.emplace<TakeExplosionDamageComponent>(entity, explosion_component);

    return entity;
}
