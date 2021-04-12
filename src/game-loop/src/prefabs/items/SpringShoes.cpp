#include "prefabs/items/SpringShoes.hpp"

#include "components/damage/GiveProjectileDamageComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/ItemComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

namespace
{
    class SpringShoesScript : public ScriptBase
    {
    public:
        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& item_component = registry.get<ItemComponent>(owner);

            if (item_component.is_carried() && !_hidden)
            {
                registry.remove<QuadComponent>(owner);
                registry.remove<MeshComponent>(owner);
                _hidden = true;
            }
        }
    private:
        bool _hidden = false;
    };
}

entt::entity prefabs::SpringShoes::create()
{
    return create(0, 0);
}

entt::entity prefabs::SpringShoes::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 0.75f;

    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    quad.frame_changed(CollectiblesSpritesheetFrames::SPRING_SHOES);

    // TODO: Rendering item icon in HUD (optionally)
    // TODO: Component for storing properties modified by passive items
    ItemComponent item(ItemType::SPRING_SHOES, ItemApplication::PASSIVE, ItemSlot::FEET);

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_3_ITEMS, CameraType::MODEL_VIEW_SPACE);
    registry.emplace<PhysicsComponent>(entity, width, height);
    registry.emplace<ItemComponent>(entity, item);
    registry.emplace<ScriptingComponent>(entity, std::make_shared<SpringShoesScript>());
    registry.emplace<HorizontalOrientationComponent>(entity);

    return entity;
}
