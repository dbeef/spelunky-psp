#include "prefabs/npc/Damsel.hpp"
#include "prefabs/particles/BonesParticle.hpp"
#include "prefabs/particles/SmokePuffParticle.hpp"
#include "prefabs/npc/DamselScript.hpp"

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
#include "components/damage/TakeProjectileDamageComponent.hpp"
#include "components/damage/HitpointComponent.hpp"
#include "components/damage/TakeMeleeDamageComponent.hpp"
#include "components/damage/TakeJumpOnTopDamage.hpp"
#include "components/damage/GiveNpcTouchDamageComponent.hpp"
#include "components/damage/TakeExplosionDamageComponent.hpp"
#include "components/generic/ItemComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/NPCSpritesheetFrames.hpp"

entt::entity prefabs::Damsel::create()
{
    return create(0, 0);
}

entt::entity prefabs::Damsel::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 1.0f;

    QuadComponent quad(TextureType::NPC, width, height);
    quad.frame_changed(NPCSpritesheetFrames::DAMSEL_STANDING);

    auto Damsel_script = std::make_shared<DamselScript>(entity);
    ScriptingComponent script(Damsel_script);

    HitpointComponent hitpoints(4, false);
    hitpoints.add_observer(reinterpret_cast<Observer<DeathEvent>*>(Damsel_script->get_observer()));

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<AnimationComponent>(entity);
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_3_ITEMS, CameraType::MODEL_VIEW_SPACE);
    registry.emplace<PhysicsComponent>(entity, width - (4.0f / 16.0f), height - (2.0f / 16.0f));
    registry.emplace<ScriptingComponent>(entity, script);
    registry.emplace<HorizontalOrientationComponent>(entity);
    registry.emplace<HitpointComponent>(entity, hitpoints);
    registry.emplace<TakeProjectileDamageComponent>(entity);
    registry.emplace<TakeSpikesDamageComponent>(entity);
    registry.emplace<NpcTypeComponent>(entity, NpcType::DAMSEL);
    registry.emplace<TakeExplosionDamageComponent>(entity);
    registry.emplace<ItemComponent>(entity, ItemType::ROCK, ItemApplication::THROWABLE, ItemSlot::ACTIVE);

    // TODO: In the original game, damsel takes melee damage from main dude, slightly jumps but no damage taken
    registry.emplace<TakeMeleeDamageComponent>(entity);

    // TODO: Does the damsel take jump on top damage? Check in the original game
    //registry.emplace<TakeJumpOnTopDamageComponent>(entity);

    return entity;
}
