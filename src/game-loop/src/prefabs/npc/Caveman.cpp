#include "prefabs/npc/Caveman.hpp"
#include "prefabs/particles/BloodParticle.hpp"

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

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/NPCSpritesheetFrames.hpp"

#include <cassert>

namespace
{
    constexpr uint16_t max_waiting_time_ms = 3000;
    constexpr uint16_t max_walking_time_ms = 5000;

    class CavemanDeathObserver final : public Observer<DieEvent>
    {
    public:

        explicit CavemanDeathObserver(entt::entity caveman) : _caveman(caveman) {}

        void on_notify(const DieEvent*) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& position = registry.get<PositionComponent>(_caveman);

            {
                auto blood = prefabs::BloodParticle::create(position.x_center, position.y_center);
                auto& physics = registry.get<PhysicsComponent>(blood);
                physics.set_x_velocity(-0.1f);
                physics.set_y_velocity(-0.1f);
            }

            {
                auto blood = prefabs::BloodParticle::create(position.x_center, position.y_center);
                auto& physics = registry.get<PhysicsComponent>(blood);
                physics.set_x_velocity(0.0f);
                physics.set_y_velocity(-0.1f);
            }

            {
                auto blood = prefabs::BloodParticle::create(position.x_center, position.y_center);
                auto& physics = registry.get<PhysicsComponent>(blood);
                physics.set_x_velocity(0.1f);
                physics.set_y_velocity(-0.1f);
            }

            registry.destroy(_caveman);
        }

    private:
        const entt::entity _caveman;
    };
    
    class CavemanScript final : public ScriptBase
    {
    public:
        explicit CavemanScript(entt::entity caveman) : _death_observer(caveman) {}

        CavemanDeathObserver* get_observer()
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

        CavemanDeathObserver _death_observer;
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

            quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::CAVEMAN_RUN_0_FIRST);
            animation.start(static_cast<std::size_t>(NPCSpritesheetFrames::CAVEMAN_RUN_0_FIRST),
                            static_cast<std::size_t>(NPCSpritesheetFrames::CAVEMAN_RUN_3_LAST),
                            110, true);
        }

        static void stop_animation(entt::entity owner)
        {
            auto& registry = EntityRegistry::instance().get_registry();

            auto& animation = registry.get<AnimationComponent>(owner);
            auto& quad = registry.get<QuadComponent>(owner);

            quad.frame_changed<NPCSpritesheetFrames>(NPCSpritesheetFrames::CAVEMAN);
            animation.stop();
        }
    };
}

entt::entity prefabs::Caveman::create()
{
    return create(0, 0);
}

entt::entity prefabs::Caveman::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 1.0f;

    QuadComponent quad(TextureType::NPC, width, height);
    quad.frame_changed(NPCSpritesheetFrames::CAVEMAN);

    auto caveman_script = std::make_shared<CavemanScript>(entity);
    ScriptingComponent script(caveman_script);

    HitpointComponent hitpoints(1);
    hitpoints.add_observer(reinterpret_cast<Observer<DieEvent>*>(caveman_script->get_observer()));

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<AnimationComponent>(entity);
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_2_ITEMS, CameraType::MODEL_VIEW_SPACE);
    registry.emplace<PhysicsComponent>(entity, width -  (4.0f / 16.0f), height -  (2.0f / 16.0f));
    registry.emplace<ScriptingComponent>(entity, script);
    registry.emplace<HorizontalOrientationComponent>(entity);
    registry.emplace<TakeProjectileDamageComponent>(entity);
    registry.emplace<TakeMeleeDamageComponent>(entity);
    registry.emplace<HitpointComponent>(entity, hitpoints);

    return entity;
}
