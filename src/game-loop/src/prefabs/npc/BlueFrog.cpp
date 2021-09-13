#include "prefabs/npc/BlueFrog.hpp"
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

#include "Vector2D.hpp"
#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/NPCSpritesheetFrames.hpp"

#include <cassert>
#include <cmath>

namespace
{
    class BlueFrogDeathObserver final : public Observer<DeathEvent>
    {
    public:

        explicit BlueFrogDeathObserver(entt::entity BlueFrog) : _blue_frog(BlueFrog) {}

        void on_notify(const DeathEvent*) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& position = registry.get<PositionComponent>(_blue_frog);

            ParticleGenerator().particle_type(ParticleType::BLOOD)
                    .position(position.x_center, position.y_center)
                    .max_velocity(0.25f, 0.25f)
                    .quantity(2)
                    .finalize();
        }

    private:
        const entt::entity _blue_frog;
    };

    class BlueFrogScript final : public ScriptBase
    {
    public:

        explicit BlueFrogScript(entt::entity blue_frog) : _death_observer(blue_frog) {}

        BlueFrogDeathObserver* get_observer()
        {
            return &_death_observer;
        }

        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& physics_component = registry.get<PhysicsComponent>(owner);
            auto& quad_component = registry.get<QuadComponent>(owner);

            _animation_timer_ms += delta_time_ms;

            if (_animation_timer_ms > 3 * 1000 && !physics_component.is_bottom_collision())
            {
                auto& animation_component = registry.get<AnimationComponent>(owner);
                animation_component.start(static_cast<std::size_t>(NPCSpritesheetFrames::BLUE_FROG_IDLE_0_START),
                                          static_cast<std::size_t>(NPCSpritesheetFrames::BLUE_FROG_IDLE_2_LAST),
                                          200, false);
                _animation_timer_ms = 0;
            }

            _look_for_main_dude_timer_ms += delta_time_ms;

            if (_look_for_main_dude_timer_ms > 3 * 1000)
            {
                const auto when_dude_is_close_callback = [&](const PositionComponent& dude_pos, const Vector2D& distance)
                {
                    auto& animation_component = registry.get<AnimationComponent>(owner);
                    animation_component.stop();
                    quad_component.frame_changed(NPCSpritesheetFrames::BLUE_FROG_JUMPING);

                    auto& physics_component = registry.get<PhysicsComponent>(owner);
                    physics_component.add_velocity(copysign(0.1f, distance.x), -0.16f);
                };

                auto& frog_position = registry.get<PositionComponent>(owner);
                main_dude_helpers::check_main_dude_proximity(when_dude_is_close_callback, {6, 4}, frog_position);

                _look_for_main_dude_timer_ms = 0;
            }
            else if (physics_component.is_bottom_collision())
            {
                quad_component.frame_changed(NPCSpritesheetFrames::BLUE_FROG_IDLE_0_START);
            }
        }

    private:

        BlueFrogDeathObserver _death_observer;
        int _animation_timer_ms = 0;
        int _look_for_main_dude_timer_ms = 0;
    };
}

entt::entity prefabs::BlueFrog::create()
{
    return create(0, 0);
}

entt::entity prefabs::BlueFrog::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 1.0f;

    QuadComponent quad(TextureType::NPC, width, height);
    quad.frame_changed(NPCSpritesheetFrames::BLUE_FROG_IDLE_0_START);

    auto BlueFrog_script = std::make_shared<BlueFrogScript>(entity);
    ScriptingComponent script(BlueFrog_script);

    HitpointComponent hitpoints(1, true);
    hitpoints.add_observer(reinterpret_cast<Observer<DeathEvent>*>(BlueFrog_script->get_observer()));

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
    registry.emplace<NpcTypeComponent>(entity, NpcType::BLUE_FROG);
    registry.emplace<GiveNpcTouchDamageComponent>(entity);
    registry.emplace<TakeExplosionDamageComponent>(entity);
    registry.emplace<TakeSpikesDamageComponent>(entity);

    auto& physics_component = registry.get<PhysicsComponent>(entity);
    physics_component.set_gravity_modifier(0.35f);
    physics_component.set_bounciness(0.25f);
    physics_component.set_friction(PhysicsComponent::get_default_friction() * 2.0f);

    return entity;
}
