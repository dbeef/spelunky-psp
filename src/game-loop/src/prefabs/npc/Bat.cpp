#include "prefabs/npc/Bat.hpp"
#include "prefabs/particles/BloodParticle.hpp"

#include "components/specialized/MainDudeComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/NpcTypeComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/damage/TakeJumpOnTopDamage.hpp"
#include "components/damage/HitpointComponent.hpp"
#include "components/damage/TakeProjectileDamageComponent.hpp"
#include "components/damage/TakeMeleeDamageComponent.hpp"

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

    class BatDeathObserver final : public Observer<DeathEvent>
    {
    public:

        explicit BatDeathObserver(entt::entity Bat) : _bat(Bat) {}

        void on_notify(const DeathEvent*) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& position = registry.get<PositionComponent>(_bat);

            for (std::size_t index = 0; index < 4; index++)
            {
                auto particle = prefabs::BloodParticle::create(position.x_center, position.y_center);
                auto& physics = registry.get<PhysicsComponent>(particle);

                float v_x = static_cast<float>(std::rand() % 2) / 15.0f;
                float v_y = static_cast<float>(std::rand() % 2) / 10.0f;

                if (std::rand() % 2)
                {
                    v_x += 0.1f;
                }
                else
                {
                    v_x -= 0.1f;
                }

                physics.set_velocity(v_x, v_y);
            }
        }

    private:
        const entt::entity _bat;
    };
    
    class BatScript final : public ScriptBase
    {
    public:
        explicit BatScript(entt::entity bat) : _death_observer(bat) {}

        BatDeathObserver* get_observer()
        {
            return &_death_observer;
        }

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
        BatDeathObserver _death_observer;
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

    auto bat_script = std::make_shared<BatScript>(entity);
    ScriptingComponent script(bat_script);

    HitpointComponent hitpoints(1);
    hitpoints.add_observer(reinterpret_cast<Observer<DeathEvent>*>(bat_script->get_observer()));

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<AnimationComponent>(entity);
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_2_ITEMS, CameraType::MODEL_VIEW_SPACE);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<ScriptingComponent>(entity, script);
    registry.emplace<HorizontalOrientationComponent>(entity);
    registry.emplace<HitpointComponent>(entity, hitpoints);
    registry.emplace<TakeProjectileDamageComponent>(entity);
    registry.emplace<TakeMeleeDamageComponent>(entity);
    registry.emplace<TakeJumpOnTopDamageComponent>(entity);
    registry.emplace<NpcTypeComponent>(entity, NpcType::BAT);

    return entity;
}
