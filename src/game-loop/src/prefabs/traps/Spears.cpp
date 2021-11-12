#include "prefabs/traps/Spears.hpp"

#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/damage/TakeExplosionDamageComponent.hpp"
#include "components/damage/GiveMeleeDamageComponent.hpp"
#include "components/damage/GiveNpcTouchDamageComponent.hpp"
#include "components/damage/GiveSpikesDamageComponent.hpp"
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

    class SpearTrapExplosionObserver : public Observer<ExplosionDamageTakenEvent>
    {
    public:
        explicit SpearTrapExplosionObserver(entt::entity owner) : _owner(owner)
        {
        }

        void on_notify(const ExplosionDamageTakenEvent* event) override
        {
            auto &registry = EntityRegistry::instance().get_registry();

            // Check if spear-trap tile was destroyed to confirm entity can be disposed:
            auto &tile_batch = Level::instance().get_tile_batch();
            auto &position = registry.get<PositionComponent>(_owner);

            auto *spear_trap_tile = tile_batch.map_tiles[static_cast<int>(position.x_center)][static_cast<int>(position.y_center)];

            // TODO
            if (spear_trap_tile->map_tile_type != MapTileType::SPEAR_TRAP_DOWN &&
                spear_trap_tile->map_tile_type != MapTileType::SPEAR_TRAP_UP)
            {
                // Can be disposed:
                registry.destroy(_owner);
            }
        }
    private:
        entt::entity _owner;
    };

    class SpearTrapScript final : public ScriptBase
    {
    public:
        explicit SpearTrapScript(entt::entity owner) : _explosion_observer(owner)
        {}

        SpearTrapExplosionObserver* get_explosion_observer()
        {
            return &_explosion_observer;
        }

        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            auto& registry = EntityRegistry::instance().get_registry();

            _update_timer_ms += delta_time_ms;

            if (_update_timer_ms < 100)
            {
                return;
            }
            else
            {
                _update_timer_ms = 0;
            }


            if (!is_triggered(owner))
            {
                auto& spears_position = registry.get<PositionComponent>(owner);
                auto& spears_physics = registry.get<PhysicsComponent>(owner);
                const auto bodies = registry.view<PhysicsComponent, PositionComponent, HitpointComponent>();

                bodies.each([&](entt::entity body, PhysicsComponent& body_physics, PositionComponent& body_position, HitpointComponent&)
                {
                    if (body == owner)
                    {
                        return;
                    }

                    if (is_triggered(owner))
                    {
                        return;
                    }

                    if (body_physics.is_collision(spears_physics, spears_position, body_position))
                    {
                        trigger(owner);
                    }
                });
            }

            update_melee_damage(owner);
        }

    private:
        static bool is_triggered(entt::entity owner)
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& animation = registry.get<AnimationComponent>(owner);
            return !animation.is_finished();
        }

        static bool deals_damage(entt::entity owner)
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& animation = registry.get<AnimationComponent>(owner);
            return animation.get_absolute_current_frame_index() >= 17 && animation.get_absolute_current_frame_index() <= 28;
        }

        static void trigger(entt::entity owner)
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& animation = registry.get<AnimationComponent>(owner);
            animation.start(static_cast<int>(NPCSpritesheetFrames::SPEARS_0_FIRST),
                            static_cast<int>(NPCSpritesheetFrames::SPEARS_30_LAST),
                            75, false);
        }

        static void update_melee_damage(entt::entity owner)
        {
            auto& registry = EntityRegistry::instance().get_registry();

            if (deals_damage(owner))
            {
                // TODO: Either new kind of damage component, or rework melee damage in DamageSystem;
                //       Upon dealing damage, should also assign speed in the opposite direction.
                // TODO: Problem with using tiles for the totem is that the flame animation can't be used.
                if (!registry.has<GiveMeleeDamageComponent>(owner)) { registry.emplace<GiveMeleeDamageComponent>(owner, 4); }
                if (!registry.has<GiveNpcTouchDamageComponent>(owner)) { registry.emplace<GiveNpcTouchDamageComponent>(owner); }
            }
            else
            {
                if (registry.has<GiveMeleeDamageComponent>(owner)) { registry.remove<GiveMeleeDamageComponent>(owner); }
                if (registry.has<GiveNpcTouchDamageComponent>(owner)) { registry.remove<GiveNpcTouchDamageComponent>(owner); }
            }
        }

        uint32_t _update_timer_ms = 0;
        SpearTrapExplosionObserver _explosion_observer;
    };
}

entt::entity prefabs::Spears::create(float pos_x_center, float pos_y_center, HorizontalOrientation orientation)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 1.0f;

    PhysicsComponent physics(width, height);
    physics.disable_gravity();

    auto spear_trap_script = std::make_shared<SpearTrapScript>(entity);

    TakeExplosionDamageComponent explosion_component;
    explosion_component.add_observer(reinterpret_cast<Observer<ExplosionDamageTakenEvent>*>(spear_trap_script->get_explosion_observer()));

    QuadComponent quad(TextureType::NPC, width, height);
    quad.frame_changed(NPCSpritesheetFrames::SPEARS_0_FIRST);
    MeshComponent mesh(RenderingLayer::LAYER_4_DUDE, CameraType::MODEL_VIEW_SPACE);

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<HorizontalOrientationComponent>(entity, orientation);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, mesh);
    registry.emplace<AnimationComponent>(entity);
    registry.emplace<ScriptingComponent>(entity, spear_trap_script);
    registry.emplace<HitpointComponent>(entity, 1, false);
    registry.emplace<TakeExplosionDamageComponent>(entity, explosion_component);

    return entity;
}
