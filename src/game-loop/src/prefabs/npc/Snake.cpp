#include "prefabs/npc/Snake.hpp"
#include "prefabs/particles/BloodParticle.hpp"
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

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/NPCSpritesheetFrames.hpp"

#include <cassert>

namespace
{
    constexpr uint16_t max_waiting_time_ms = 3000;
    constexpr uint16_t max_walking_time_ms = 5000;

    class SnakeDeathObserver final : public Observer<DeathEvent>
    {
    public:

        explicit SnakeDeathObserver(entt::entity snake) : _snake(snake) {}

        void on_notify(const DeathEvent*) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& position = registry.get<PositionComponent>(_snake);

            ParticleGenerator().particle_type(ParticleType::BLOOD)
                               .position(position.x_center, position.y_center)
                               .max_velocity(0.25f, 0.25f)
                               .quantity(2)
                               .finalize();
        }

    private:
        const entt::entity _snake;
    };
    
    class SnakeScript final : public ScriptBase
    {
    public:

        explicit SnakeScript(entt::entity snake) : _death_observer(snake) {}

        SnakeDeathObserver* get_observer()
        {
            return &_death_observer;
        }

        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            auto& registry = EntityRegistry::instance().get_registry();

            if (_walking_timer_ms > 0)
            {
               _walking_timer_ms -= delta_time_ms;

               if (_walking_timer_ms <= 0)
               {
                   _waiting_timer_ms = std::rand() % max_waiting_time_ms;
                   stop_animation(owner);
               }

                walk(owner);
            }

            if (_waiting_timer_ms >= 0)
            {
                _waiting_timer_ms -= delta_time_ms;

                if (_waiting_timer_ms < 0)
                {
                    _walking_timer_ms = std::rand() % max_walking_time_ms;
                    start_animation(owner);
                    randomize_orientation(owner);
                }
            }
        }

    private:

        SnakeDeathObserver _death_observer;
        int _walking_timer_ms = 0;
        int _waiting_timer_ms = 0;

        static void walk(entt::entity owner)
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& orientation = registry.get<HorizontalOrientationComponent>(owner);
            auto& physics = registry.get<PhysicsComponent>(owner);

            switch (orientation.orientation)
            {
                case HorizontalOrientation::LEFT: physics.set_x_velocity(-0.01f); break;
                case HorizontalOrientation::RIGHT: physics.set_x_velocity(0.01f); break;
                default: assert(false);
            }
        }

        static void randomize_orientation(entt::entity owner)
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& orientation = registry.get<HorizontalOrientationComponent>(owner);
            orientation.orientation = static_cast<HorizontalOrientation>(std::rand() % 2);
        }

        static void start_animation(entt::entity owner)
        {
            auto& registry = EntityRegistry::instance().get_registry();

            auto& animation = registry.get<AnimationComponent>(owner);
            auto& quad = registry.get<QuadComponent>(owner);

            quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::SNAKE_WALK_0_FIRST);
            animation.start(static_cast<std::size_t>(NPCSpritesheetFrames::SNAKE_WALK_0_FIRST),
                            static_cast<std::size_t>(NPCSpritesheetFrames::SNAKE_WALK_3_LAST),
                            110, true);
        }

        static void stop_animation(entt::entity owner)
        {
            auto& registry = EntityRegistry::instance().get_registry();

            auto& animation = registry.get<AnimationComponent>(owner);
            auto& quad = registry.get<QuadComponent>(owner);

            quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::SNAKE);
            animation.stop();
        }
    };
}

entt::entity prefabs::Snake::create()
{
    return create(0, 0);
}

entt::entity prefabs::Snake::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 1.0f;

    QuadComponent quad(TextureType::NPC, width, height);
    quad.frame_changed(NPCSpritesheetFrames::SNAKE);

    auto snake_script = std::make_shared<SnakeScript>(entity);
    ScriptingComponent script(snake_script);

    HitpointComponent hitpoints(1, true);
    hitpoints.add_observer(reinterpret_cast<Observer<DeathEvent>*>(snake_script->get_observer()));

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<AnimationComponent>(entity);
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_3_ITEMS, CameraType::MODEL_VIEW_SPACE);
    registry.emplace<PhysicsComponent>(entity, width - (4.0f / 16.0f), height - (4.0f / 16.0f));
    registry.emplace<ScriptingComponent>(entity, script);
    registry.emplace<HorizontalOrientationComponent>(entity);
    registry.emplace<HitpointComponent>(entity, hitpoints);
    registry.emplace<TakeProjectileDamageComponent>(entity);
    registry.emplace<TakeMeleeDamageComponent>(entity);
    registry.emplace<TakeJumpOnTopDamageComponent>(entity);
    registry.emplace<NpcTypeComponent>(entity, NpcType::SNAKE);
    registry.emplace<GiveNpcTouchDamageComponent>(entity);
    registry.emplace<TakeExplosionDamageComponent>(entity);
    registry.emplace<TakeSpikesDamageComponent>(entity);

    return entity;
}
