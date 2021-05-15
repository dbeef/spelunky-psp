#include "prefabs/items/Flare.hpp"

#include "components/damage/GiveProjectileDamageComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/ItemComponent.hpp"
#include "components/generic/ParticleEmitterComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

entt::entity prefabs::Flare::create()
{
    return create(0, 0);
}

entt::entity prefabs::Flare::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 0.5f;
    const float height = 0.5f;

    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    quad.frame_changed(CollectiblesSpritesheetFrames::FLARE_0_FIRST);

    AnimationComponent animation;
    animation.start(static_cast<int>(CollectiblesSpritesheetFrames::FLARE_0_FIRST),
                    static_cast<int>(CollectiblesSpritesheetFrames::FLARE_1_LAST),
                    100, true);

    PhysicsComponent physics(width, height);
    physics.set_friction(0.02f);
    physics.set_bounciness(0.55f);

    ItemComponent item(ItemType::FLARE, ItemApplication::THROWABLE, ItemSlot::ACTIVE);
    item.set_weight(2);

    ParticleEmitterComponent particle_emitter{};
    particle_emitter.particle_type = ParticleType::FLARE_SPARK;
    particle_emitter.interval_ms = 100;

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_3_ITEMS, CameraType::MODEL_VIEW_SPACE);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<ItemComponent>(entity, item);
    registry.emplace<AnimationComponent>(entity, animation);
    registry.emplace<HorizontalOrientationComponent>(entity);
    registry.emplace<GiveProjectileDamageComponent>(entity, 1);
    registry.emplace<ParticleEmitterComponent>(entity, particle_emitter);

    return entity;
}
