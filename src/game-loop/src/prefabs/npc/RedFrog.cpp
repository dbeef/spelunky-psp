#include "prefabs/npc/RedFrog.hpp"
#include "other/ParticleGenerator.hpp"

#include "components/generic/AnimationComponent.hpp"
#include "components/generic/NpcTypeComponent.hpp"
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
#include "main-dude/MainDudeHelpers.hpp"
#include "other/ParticleGenerator.hpp"
#include "prefabs/effects/Explosion.hpp"

#include "Vector2D.hpp"
#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/NPCSpritesheetFrames.hpp"
#include "audio/Audio.hpp"

#include <cassert>
#include <cmath>

namespace
{
    class RedFrogDeathObserver final : public Observer<DeathEvent>
    {
    public:
        explicit RedFrogDeathObserver(entt::entity red_frog) : _red_frog(red_frog) {}
        void on_notify(const DeathEvent*) override;
    private:
        const entt::entity _red_frog;
    };

    class RedFrogScript final : public ScriptBase
    {
    public:

        explicit RedFrogScript(entt::entity red_frog) : _death_observer(red_frog) {}

        RedFrogDeathObserver* get_observer()
        {
            return &_death_observer;
        }

        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& physics_component = registry.get<PhysicsComponent>(owner);
            auto& quad_component = registry.get<QuadComponent>(owner);
            auto& position = registry.get<PositionComponent>(owner);

            if (_triggered)
            {
                _triggered_timer_ms += delta_time_ms;

                if (_triggered_timer_ms > 3 * 1000)
                {
                    ParticleGenerator().particle_type(ParticleType::BLOOD)
                            .position(position.x_center, position.y_center)
                            .max_velocity(0.25f, 0.25f)
                            .quantity(2)
                            .finalize();

                    ParticleGenerator().particle_type(ParticleType::FLAME)
                            .position(position.x_center, position.y_center)
                            .max_velocity(0.25f, 0.25f)
                            .quantity(4)
                            .finalize();

                    prefabs::Explosion::create(position.x_center, position.y_center);

                    Audio::instance().play(SFXType::EXPLOSION);

                    registry.destroy(owner);
                }

                return;
            }

            _look_for_main_dude_timer_ms += delta_time_ms;
            _animation_timer_ms += delta_time_ms;

            if (_look_for_main_dude_timer_ms > 3 * 1000)
            {
                const auto when_dude_is_close_callback = [&](const PositionComponent& dude_pos, const Vector2D& distance)
                {
                    auto& animation_component = registry.get<AnimationComponent>(owner);
                    animation_component.stop();
                    quad_component.frame_changed(NPCSpritesheetFrames::RED_FROG_JUMPING);

                    auto& physics_component = registry.get<PhysicsComponent>(owner);
                    physics_component.add_velocity(copysign(0.1f, distance.x), -0.16f);
                };

                auto& frog_position = registry.get<PositionComponent>(owner);
                main_dude_helpers::check_main_dude_proximity(when_dude_is_close_callback, {6, 4}, frog_position);

                _look_for_main_dude_timer_ms = 0;
            }
            else if (physics_component.is_bottom_collision())
            {
                if (_animation_timer_ms >= 3 * 1000)
                {
                    _animation_timer_ms = 0;
                    quad_component.frame_changed(NPCSpritesheetFrames::RED_FROG_IDLE_0_START);
                    auto& animation_component = registry.get<AnimationComponent>(owner);
                    animation_component.start(static_cast<std::size_t>(NPCSpritesheetFrames::RED_FROG_IDLE_0_START),
                                              static_cast<std::size_t>(NPCSpritesheetFrames::RED_FROG_IDLE_5_LAST),
                                              175, true);
                }
            }
        }

        void trigger(entt::entity owner)
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& quad = registry.get<QuadComponent>(owner);
            quad.frame_changed(NPCSpritesheetFrames::RED_FROG_ARMED_0_FIRST);
            auto& animation = registry.get<AnimationComponent>(owner);
            animation.start(static_cast<std::size_t>(NPCSpritesheetFrames::RED_FROG_ARMED_0_FIRST),
                            static_cast<std::size_t>(NPCSpritesheetFrames::RED_FROG_ARMED_2_LAST),
                            100, true);
            _triggered = true;
        }

    private:

        bool _triggered = false;
        int _triggered_timer_ms = 0;
        RedFrogDeathObserver _death_observer;
        int _look_for_main_dude_timer_ms = 0;
        int _animation_timer_ms = 0;
    };

    void RedFrogDeathObserver::on_notify(const DeathEvent *)
    {
        // TODO: Check if a frog is supposed to die instantly upon explosion damage,
        //       if so, add an explosion damage observer checking for that case specifically

        auto& registry = EntityRegistry::instance().get_registry();
        auto& scripting_component = registry.get<ScriptingComponent>(_red_frog);
        auto* red_frog_script = scripting_component.get<RedFrogScript>();

        red_frog_script->trigger(_red_frog);
    }
}

entt::entity prefabs::RedFrog::create()
{
    return create(0, 0);
}

entt::entity prefabs::RedFrog::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 1.0f;

    QuadComponent quad(TextureType::NPC, width, height);
    quad.frame_changed(NPCSpritesheetFrames::RED_FROG_IDLE_0_START);

    auto RedFrog_script = std::make_shared<RedFrogScript>(entity);
    ScriptingComponent script(RedFrog_script);

    HitpointComponent hitpoints(1, false);
    hitpoints.add_observer(reinterpret_cast<Observer<DeathEvent>*>(RedFrog_script->get_observer()));

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<AnimationComponent>(entity);
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_3_ITEMS, CameraType::MODEL_VIEW_SPACE);
    registry.emplace<PhysicsComponent>(entity, width, height - (4 * 1.0f/16.0f));
    registry.emplace<ScriptingComponent>(entity, script);
    registry.emplace<HorizontalOrientationComponent>(entity);
    registry.emplace<HitpointComponent>(entity, hitpoints);
    registry.emplace<TakeProjectileDamageComponent>(entity);
    registry.emplace<TakeMeleeDamageComponent>(entity);
    registry.emplace<TakeJumpOnTopDamageComponent>(entity);
    registry.emplace<NpcTypeComponent>(entity, NpcType::RED_FROG);
    registry.emplace<GiveNpcTouchDamageComponent>(entity);
    registry.emplace<TakeExplosionDamageComponent>(entity);
    registry.emplace<TakeSpikesDamageComponent>(entity);

    auto& physics_component = registry.get<PhysicsComponent>(entity);
    physics_component.set_gravity_modifier(0.35f);
    physics_component.set_bounciness(0.25f);
    physics_component.set_friction(PhysicsComponent::get_default_friction() * 2.0f);

    auto& animation_component = registry.get<AnimationComponent>(entity);
    animation_component.start(static_cast<std::size_t>(NPCSpritesheetFrames::RED_FROG_IDLE_0_START),
                              static_cast<std::size_t>(NPCSpritesheetFrames::RED_FROG_IDLE_5_LAST),
                              250, true);

    return entity;
}
