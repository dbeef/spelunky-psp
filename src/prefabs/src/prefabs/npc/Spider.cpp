#include "prefabs/npc/Spider.hpp"
#include "prefabs/particles/BloodParticle.hpp"
#include "other/ParticleGenerator.hpp"

#include "components/specialized/MainDudeComponent.hpp"
#include "components/generic/NpcTypeComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/damage/HitpointComponent.hpp"
#include "components/damage/TakeProjectileDamageComponent.hpp"
#include "components/damage/TakeMeleeDamageComponent.hpp"
#include "components/damage/TakeJumpOnTopDamage.hpp"
#include "components/damage/GiveNpcTouchDamageComponent.hpp"
#include "components/damage/TakeExplosionDamageComponent.hpp"
#include "components/damage/TakeSpikesDamageComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/NPCSpritesheetFrames.hpp"

#include <cmath>

namespace
{
    constexpr float activation_distance_x = 0.5f;
    constexpr float activation_distance_y = 7;
    constexpr uint32_t update_delta_ms = 20;
   
    class SpiderDeathObserver final : public Observer<DeathEvent>
    {
    public:

        explicit SpiderDeathObserver(entt::entity Spider) : _spider(Spider) {}

        void on_notify(const DeathEvent*) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& position = registry.get<PositionComponent>(_spider);

            ParticleGenerator().particle_type(ParticleType::BLOOD)
                               .position(position.x_center, position.y_center)
                               .max_velocity(0.25f, 0.25f)
                               .quantity(2)
                               .finalize();
        }

    private:
        const entt::entity _spider;
    };

    class SpiderScript final : public ScriptBase
    {
    public:

        explicit SpiderScript(entt::entity spider)
            : _death_observer(spider)
        {
        }

        SpiderDeathObserver* get_observer()
        {
            return &_death_observer;
        }

        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            _update_timer_ms += delta_time_ms;
            if (_update_timer_ms < update_delta_ms)
            {
                return;
            }
            _update_timer_ms = 0;

            auto& registry = EntityRegistry::instance().get_registry();

            const auto &spider_position = registry.get<PositionComponent>(owner);
            const auto main_dudes = registry.view<MainDudeComponent, PositionComponent>();
            main_dudes.each([&](MainDudeComponent &, PositionComponent &dude_position) {
                if (!_triggered && dude_position.y_center < spider_position.y_center)
                {
                    return;
                }

                float distance_x = std::fabs(dude_position.x_center - spider_position.x_center);
                float distance_y = std::fabs(dude_position.y_center - spider_position.y_center);

                if (_triggered || distance_x < activation_distance_x && distance_y < activation_distance_y)
                {
                    _dude_position = {dude_position.x_center, dude_position.y_center};

                    if (!_triggered)
                    {
                        trigger(owner);
                    }
                }
            });

            if (_triggered && !_flipping_animation_finished)
            {
                auto& animation = registry.get<AnimationComponent>(owner);
                if (animation.is_finished())
                {
                    animation.start(static_cast<std::size_t>(NPCSpritesheetFrames::SPIDER_TRIGGERED_0_FIRST),
                                    static_cast<std::size_t>(NPCSpritesheetFrames::SPIDER_TRIGGERED_3_LAST),
                                    100, true);
                    _flipping_animation_finished = true;
                }
            }
            else if (_triggered && _flipping_animation_finished)
            {
                auto& physics = registry.get<PhysicsComponent>(owner);
                auto& position = registry.get<PositionComponent>(owner);
                _jump_cooldown_ms += delta_time_ms;

                if (_jump_cooldown_ms > 200)
                {
                    _jump_cooldown_ms = 0;

                    if (physics.is_bottom_collision())
                    {
                        physics.set_y_velocity(-0.14f);
                    }

                    if (position.x_center > _dude_position.x)
                    {
                        physics.set_x_velocity(-0.05f);
                    }
                    else
                    {
                        physics.set_x_velocity(0.05f);
                    }
                }
            }
        }

        void trigger(entt::entity owner)
        {
            _triggered = true;

            auto& registry = EntityRegistry::instance().get_registry();

            auto &quad = registry.get<QuadComponent>(owner);
            auto &animation = registry.get<AnimationComponent>(owner);
            auto &physics = registry.get<PhysicsComponent>(owner);

            physics.enable_gravity();
            quad.frame_changed(NPCSpritesheetFrames::SPIDER_FLIP_0_FIRST);
            animation.start(static_cast<std::size_t>(NPCSpritesheetFrames::SPIDER_FLIP_0_FIRST),
                            static_cast<std::size_t>(NPCSpritesheetFrames::SPIDER_FLIP_8_LAST),
                            100, false);
        }

    private:

        SpiderDeathObserver _death_observer;
        bool _triggered = false;
        bool _flipping_animation_finished = false;
        Point2D _dude_position;
        uint32_t _update_timer_ms = 0;
        uint32_t _jump_cooldown_ms = 0;
    };
}

entt::entity prefabs::Spider::create()
{
    return create(0, 0);
}

entt::entity prefabs::Spider::create(float pos_x_center, float pos_y_center, bool triggered)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 1.0f;

    QuadComponent quad(TextureType::NPC, width, height);
    quad.frame_changed(NPCSpritesheetFrames::SPIDER_HANG);

    PhysicsComponent physics(width  - (2.0f / 16.0f), height - (2.0f / 16.0f));
    physics.set_gravity_modifier(0.45f);
    physics.set_bounciness(0.6f);
    physics.disable_gravity();

    auto spider_script = std::make_shared<SpiderScript>(entity);
    ScriptingComponent script(spider_script);

    HitpointComponent hitpoints(1, true);
    hitpoints.add_observer(reinterpret_cast<Observer<DeathEvent>*>(spider_script->get_observer()));

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<AnimationComponent>(entity);
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_3_ITEMS, CameraType::MODEL_VIEW_SPACE);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<ScriptingComponent>(entity, spider_script);
    registry.emplace<HorizontalOrientationComponent>(entity);
    registry.emplace<HitpointComponent>(entity, hitpoints);
    registry.emplace<TakeProjectileDamageComponent>(entity);
    registry.emplace<TakeMeleeDamageComponent>(entity);
    registry.emplace<TakeJumpOnTopDamageComponent>(entity);
    registry.emplace<NpcTypeComponent>(entity, NpcType::SPIDER);
    registry.emplace<GiveNpcTouchDamageComponent>(entity);
    registry.emplace<TakeExplosionDamageComponent>(entity);
    registry.emplace<TakeSpikesDamageComponent>(entity);

    if (triggered)
    {
        spider_script->trigger(entity);
    }

    return entity;
}
