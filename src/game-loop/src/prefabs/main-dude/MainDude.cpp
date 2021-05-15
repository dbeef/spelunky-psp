#include "prefabs/main-dude/MainDude.hpp"
#include "prefabs/items/BombSpawner.hpp"
#include "prefabs/items/RopeSpawner.hpp"
#include "prefabs/items/Wallet.hpp"
#include "prefabs/items/Whip.hpp"

#include "components/generic/PositionComponent.hpp"
#include "components/generic/ClimbingComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/InputComponent.hpp"
#include "components/damage/HitpointComponent.hpp"
#include "components/damage/TakeNpcTouchDamageComponent.hpp"
#include "components/damage/GiveJumpOnTopDamage.hpp"
#include "components/damage/TakeExplosionDamageComponent.hpp"
#include "components/damage/TakeFallDamageComponent.hpp"
#include "components/damage/TakeProjectileDamageComponent.hpp"
#include "components/damage/TakeSpikesDamageComponent.hpp"
#include "components/specialized/MainDudeComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "other/Inventory.hpp"

namespace
{
    static constexpr float DEFAULT_MAX_Y_VELOCITY = 0.39f;
}

namespace prefabs
{
    entt::entity MainDude::create()
    {
        return create(0, 0);
    }

    entt::entity MainDude::create(float pos_x_center, float pos_y_center)
    {
        auto &registry = EntityRegistry::instance().get_registry();

        const auto entity = registry.create();

        PhysicsComponent physics(1 - (2.0f / 16.0f), 1 - (1.0f / 16.0f));

        PositionComponent position(pos_x_center, pos_y_center);
        QuadComponent quad(TextureType::MAIN_DUDE, 1, 1);
        AnimationComponent animation;
        InputComponent input;
        ItemCarrierComponent item_carrier;

        MeshComponent mesh;
        mesh.camera_type = CameraType::MODEL_VIEW_SPACE;
        mesh.rendering_layer = RenderingLayer::LAYER_4_DUDE;

        registry.emplace<PositionComponent>(entity, position);
        registry.emplace<QuadComponent>(entity, quad);
        registry.emplace<AnimationComponent>(entity, animation);
        registry.emplace<PhysicsComponent>(entity, physics);
        registry.emplace<MeshComponent>(entity, mesh);
        registry.emplace<InputComponent>(entity, input);
        registry.emplace<HorizontalOrientationComponent>(entity);
        registry.emplace<ItemCarrierComponent>(entity, item_carrier);
        registry.emplace<GiveJumpOnTopDamageComponent>(entity, 1);

        MainDudeComponent main_dude(entity);
        registry.emplace<MainDudeComponent>(entity, main_dude);

        HitpointComponent hitpoints(Inventory::instance().get_hearts(), false);
        hitpoints.add_observer(reinterpret_cast<Observer<DeathEvent> *>(main_dude.get_death_observer()));
        registry.emplace<HitpointComponent>(entity, hitpoints);

        ClimbingComponent climbing;
        climbing.add_observer(reinterpret_cast<Observer<ClimbingEvent> *>(main_dude.get_climbing_observer()));
        registry.emplace<ClimbingComponent>(entity, climbing);

        TakeFallDamageComponent take_fall_damage(1, DEFAULT_MAX_Y_VELOCITY);
        take_fall_damage.add_observer(reinterpret_cast<Observer<FallDamage_t> *>(main_dude.get_fall_observer()));
        registry.emplace<TakeFallDamageComponent>(entity, take_fall_damage);

        TakeNpcTouchDamageComponent take_npc_damage;
        take_npc_damage.add_observer(reinterpret_cast<Observer<NpcDamage_t> *>(main_dude.get_npc_damage_observer()));
        registry.emplace<TakeNpcTouchDamageComponent>(entity, take_npc_damage);

        TakeExplosionDamageComponent take_explosion_damage;
        take_explosion_damage.add_observer(reinterpret_cast<Observer<ExplosionDamageTakenEvent> *>(main_dude.get_explosion_damage_observer()));
        registry.emplace<TakeExplosionDamageComponent>(entity, take_explosion_damage);

        TakeProjectileDamageComponent take_projectile_damage;
        take_projectile_damage.add_observer(reinterpret_cast<Observer<TakenProjectileDamageEvent> *>(main_dude.get_projectile_damage_observer()));
        registry.emplace<TakeProjectileDamageComponent>(entity, take_projectile_damage);

        TakeSpikesDamageComponent take_spikes_damage;
        take_spikes_damage.add_observer(reinterpret_cast<Observer<SpikesDamageEvent> *>(main_dude.get_spikes_damage_observer()));
        registry.emplace<TakeSpikesDamageComponent>(entity, take_spikes_damage);

        {
            auto& carrier = registry.get<ItemCarrierComponent>(entity);

            auto bomb_spawner_entity = prefabs::BombSpawner::create(Inventory::instance().get_bombs());
            auto& bomb_spawner_scripting_component = registry.get<ScriptingComponent>(bomb_spawner_entity);
            auto* bomb_spawner_script = bomb_spawner_scripting_component.get<prefabs::BombSpawner::BombSpawnerScript>();

            auto rope_spawner_entity = prefabs::RopeSpawner::create(Inventory::instance().get_ropes());
            auto& rope_spawner_scripting_component = registry.get<ScriptingComponent>(rope_spawner_entity);
            auto* rope_spawner_script = rope_spawner_scripting_component.get<prefabs::RopeSpawner::RopeSpawnerScript>();

            carrier.pick_up_item(bomb_spawner_entity, entity);
            carrier.pick_up_item(rope_spawner_entity, entity);
            carrier.pick_up_item(prefabs::Whip::create(pos_x_center, pos_y_center), entity);
            carrier.pick_up_item(prefabs::Wallet::create(), entity);

            auto& inventory = Inventory::instance();
            bomb_spawner_script->add_observer(&inventory);
            rope_spawner_script->add_observer(&inventory);
        }

        return entity;
    }
}
