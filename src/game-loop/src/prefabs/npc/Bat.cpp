#include "prefabs/npc/Bat.hpp"

#include "components/specialized/MainDudeComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/NPCSpritesheetFrames.hpp"

#include <cmath>

namespace
{
    constexpr uint32_t update_delta_ms = 20;
    constexpr float activation_distance_x = 7;
    constexpr float activation_distance_y = 6;
    constexpr float x_speed = 0.02f;
    constexpr float y_speed = 0.02f;

    class BatScript : public ScriptBase
    {
    public:

        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            // TODO: ScriptingComponent member field to define update intervals?
            _update_timer_ms += delta_time_ms;
            if (_update_timer_ms < update_delta_ms)
            {
                return;
            }
            _update_timer_ms = 0;

            auto& registry = EntityRegistry::instance().get_registry();
            const auto& bat_position = registry.get<PositionComponent>(owner);

            const bool triggered_before = _triggered;
            _triggered = false;
            const auto main_dudes = registry.view<MainDudeComponent, PositionComponent>();
            main_dudes.each([&](MainDudeComponent&, PositionComponent& dude_position)
            {
                if (_triggered)
                {
                    return;
                }

                if (dude_position.y_center < bat_position.y_center && !triggered_before)
                {
                    return;
                }

                _distance_x = std::fabs(dude_position.x_center - bat_position.x_center);
                _distance_y = std::fabs(dude_position.y_center - bat_position.y_center);

                if (_distance_x < activation_distance_x && _distance_y < activation_distance_y)
                {
                    _flying = true;
                    _triggered = true;
                    _dude_position = {dude_position.x_center, dude_position.y_center};
                }
            });

            if (_triggered)
            {
                auto& horizontal_orientation = registry.get<HorizontalOrientationComponent>(owner);
                auto& bat_physics = registry.get<PhysicsComponent>(owner);

                if (_distance_x > 0.1f) // To not wobble.
                {
                    if (_dude_position.x > bat_position.x_center)
                    {
                        horizontal_orientation.orientation = HorizontalOrientation::RIGHT;
                        bat_physics.set_x_velocity(x_speed);
                    }
                    else
                    {
                        horizontal_orientation.orientation = HorizontalOrientation::LEFT;
                        bat_physics.set_x_velocity(-x_speed);
                    }
                }

                if (_distance_y > 0.1f) // To not wobble.
                {
                    if (_dude_position.y > bat_position.y_center)
                    {
                        bat_physics.set_y_velocity(y_speed);
                    }
                    else
                    {
                        bat_physics.set_y_velocity(-y_speed);
                    }
                }
            }
            else
            {
                auto& bat_physics = registry.get<PhysicsComponent>(owner);
                if (bat_physics.is_upper_collision())
                {
                    _flying = false;
                    bat_physics.set_velocity(0, 0);
                }
                else
                {
                    bat_physics.set_y_velocity(-y_speed);
                    _flying = true;
                }
            }

            if (_flying && !_flying_animation_started)
            {
                auto& quad = registry.get<QuadComponent>(owner);
                auto& animation = registry.get<AnimationComponent>(owner);

                quad.frame_changed(NPCSpritesheetFrames::BAT_FLYING_0_FIRST);
                animation.start(static_cast<std::size_t>(NPCSpritesheetFrames::BAT_FLYING_0_FIRST),
                                static_cast<std::size_t>(NPCSpritesheetFrames::BAT_FLYING_2_LAST),
                100, true);

                _flying_animation_started = true;
            }
            else if (!_flying && _flying_animation_started)
            {
                auto& quad = registry.get<QuadComponent>(owner);
                auto& animation = registry.get<AnimationComponent>(owner);

                quad.frame_changed(NPCSpritesheetFrames::BAT_HANG);
                animation.stop();

                _flying_animation_started = false;
            }
        }
    private:
        bool _flying = false;
        bool _flying_animation_started = false;
        bool _triggered = false;
        uint32_t _update_timer_ms = 0;
        float _distance_x = 0.0f;
        float _distance_y = 0.0f;
        Point2D _dude_position;
    };
}

entt::entity prefabs::Bat::create()
{
    return create(0, 0);
}

entt::entity prefabs::Bat::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 1.0f;

    QuadComponent quad(TextureType::NPC, width, height);
    quad.frame_changed(NPCSpritesheetFrames::BAT_HANG);

    PhysicsComponent physics(width  - (2.0f / 16.0f), height - (2.0f / 16.0f));
    physics.disable_gravity();

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<AnimationComponent>(entity);
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_2_ITEMS, CameraType::MODEL_VIEW_SPACE);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<ScriptingComponent>(entity, std::make_shared<BatScript>());
    registry.emplace<HorizontalOrientationComponent>(entity);

    return entity;
}
