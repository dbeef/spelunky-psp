#include "prefabs/npc/Shopkeeper.hpp"
#include "prefabs/particles/BonesParticle.hpp"
#include "prefabs/particles/SmokePuffParticle.hpp"
#include "prefabs/npc/ShopkeeperScript.hpp"

#include "other/ParticleGenerator.hpp"

#include "components/specialized/MainDudeComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/NpcTypeComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/damage/TakeFallDamageComponent.hpp"
#include "components/damage/GiveProjectileDamageComponent.hpp"
#include "components/damage/TakeProjectileDamageComponent.hpp"
#include "components/damage/TakeSpikesDamageComponent.hpp"
#include "components/damage/HitpointComponent.hpp"
#include "components/damage/TakeMeleeDamageComponent.hpp"
#include "components/damage/TakeJumpOnTopDamage.hpp"
#include "components/damage/GiveNpcTouchDamageComponent.hpp"
#include "components/damage/TakeExplosionDamageComponent.hpp"
#include "components/generic/ItemComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/NPCSpritesheetFrames.hpp"

entt::entity prefabs::Shopkeeper::create()
{
    return create(0, 0);
}

entt::entity prefabs::Shopkeeper::create(float pos_x_center, float pos_y_center) {
    auto &registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 1.0f;

    QuadComponent quad(TextureType::NPC, width, height);
    quad.frame_changed(NPCSpritesheetFrames::SHOPKEEPER_STANDING);

    auto shopkeeper_script = std::make_shared<ShopkeeperScript>(entity);
    ScriptingComponent script(shopkeeper_script);

    HitpointComponent hitpoints(4, false);
    hitpoints.add_observer(reinterpret_cast<Observer<DeathEvent> *>(shopkeeper_script->get_death_observer()));

    TakeProjectileDamageComponent take_projectile_damage;
    take_projectile_damage.add_observer(reinterpret_cast<Observer<TakenProjectileDamageEvent> *>(shopkeeper_script->get_projectile_damage_observer()));

    TakeMeleeDamageComponent take_melee_damage;
    take_melee_damage.add_observer(reinterpret_cast<Observer<TakenMeleeDamageEvent> *>(shopkeeper_script->get_melee_damage_observer()));

    TakeJumpOnTopDamageComponent take_jump_on_top_damage;
    take_jump_on_top_damage.add_observer(reinterpret_cast<Observer<TakenJumpOnTopDamageEvent> *>(shopkeeper_script->get_jump_on_top_damage_observer()));

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<AnimationComponent>(entity);
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_3_ITEMS, CameraType::MODEL_VIEW_SPACE);
    registry.emplace<PhysicsComponent>(entity, width - (4.0f / 16.0f), height - (2.0f / 16.0f));
    registry.emplace<ScriptingComponent>(entity, script);
    registry.emplace<HorizontalOrientationComponent>(entity);
    registry.emplace<HitpointComponent>(entity, hitpoints);
    registry.emplace<TakeProjectileDamageComponent>(entity, take_projectile_damage);
    registry.emplace<TakeSpikesDamageComponent>(entity);
    registry.emplace<TakeMeleeDamageComponent>(entity, take_melee_damage);
    registry.emplace<TakeJumpOnTopDamageComponent>(entity, take_jump_on_top_damage);
    registry.emplace<NpcTypeComponent>(entity, NpcType::SHOPKEEPER);
    registry.emplace<TakeExplosionDamageComponent>(entity);
    registry.emplace<ItemCarrierComponent>(entity);

    return entity;
}
