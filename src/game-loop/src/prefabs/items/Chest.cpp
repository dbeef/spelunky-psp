#include "prefabs/items/Chest.hpp"
#include "prefabs/collectibles/SmallGem.hpp"

#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/ItemComponent.hpp"
#include "components/generic/OpenableComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/damage/GiveProjectileDamageComponent.hpp"

#include "audio/Audio.hpp"
#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

namespace
{
    class ChestScript final : public ScriptBase
    {
    public:

        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& openable = registry.get<OpenableComponent>(owner);

            if (openable.opened && !_loot_ejected)
            {
                auto& quad = registry.get<QuadComponent>(owner);
                quad.frame_changed(CollectiblesSpritesheetFrames::CHEST_OPENED);

                Audio::instance().play(SFXType::CHEST_OPEN);

                // TODO: Cooldown for collectibles before they can be collected.

                auto& position = registry.get<PositionComponent>(owner);

                {
                    auto gem = prefabs::SmallGem::create(position.x_center, position.y_center);
                    auto& physics = registry.get<PhysicsComponent>(gem);
                    physics.set_velocity(0, -0.1f);
                }

                {
                    auto gem = prefabs::SmallGem::create(position.x_center, position.y_center);
                    auto& physics = registry.get<PhysicsComponent>(gem);
                    physics.set_velocity(-0.1f, -0.1f);
                }

                {
                    auto gem = prefabs::SmallGem::create(position.x_center, position.y_center);
                    auto& physics = registry.get<PhysicsComponent>(gem);
                    physics.set_velocity(0.1f, -0.1f);
                }

                _loot_ejected = true;
            }
        }
    private:
        bool _loot_ejected = false;
    };
}

entt::entity prefabs::Chest::create()
{
    return create(0, 0);
}

entt::entity prefabs::Chest::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 1.0f;

    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    quad.frame_changed(CollectiblesSpritesheetFrames::CHEST);

    PhysicsComponent physics(width, height);
    physics.set_friction(0.02f);
    physics.set_bounciness(0.25f);

    ItemComponent item(ItemType::CHEST, ItemApplication::OPENABLE, ItemSlot::ACTIVE);
    item.set_weight(5);
    item.set_carrying_offset({0.0f, -0.3f});

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_3_ITEMS, CameraType::MODEL_VIEW_SPACE);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<ItemComponent>(entity, item);
    registry.emplace<ScriptingComponent>(entity, std::make_shared<ChestScript>());
    registry.emplace<HorizontalOrientationComponent>(entity);
    registry.emplace<OpenableComponent>(entity);
    registry.emplace<GiveProjectileDamageComponent>(entity, 1);

    return entity;
}
