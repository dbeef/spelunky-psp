#include "prefabs/items/Jetpack.hpp"
#include "prefabs/particles/PoofParticle.hpp"

#include "components/generic/ItemComponent.hpp"
#include "components/generic/ActivableComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"

#include "EntityRegistry.hpp"
#include "audio/Audio.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

namespace
{
    constexpr uint32_t max_fuel_activations = 6;
    constexpr uint32_t fuel_regeneration_delta_ms = 500;

    class JetpackScript final : public ScriptBase
    {
    public:
        // TODO: Show icon when in inventory (but only in main-dude's inventory, not when i.e some other thing carries it)
        // ^ HUD system for tracking main dude inventory? Sounds better than InventoryEntity
        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& item = registry.get<ItemComponent>(owner);

            if (!item.is_carried())
            {
                return;
            }

            auto& activable = registry.get<ActivableComponent>(owner);
            auto& position = registry.get<PositionComponent>(owner);

            _fuel_regeneration_timer_ms += delta_time_ms;
            _activation_timer_ms += delta_time_ms;
            _cooldown_timer_ms = std::max<uint32_t>(0, static_cast<int>(_cooldown_timer_ms) - delta_time_ms);

            if (_fuel_regeneration_timer_ms > fuel_regeneration_delta_ms)
            {
                _fuel_regeneration_timer_ms = 0;
                _fuel_activations = std::min<uint32_t>(_fuel_activations + 1, max_fuel_activations);
            }

            if (activable.activated)
            {
                if (_activation_timer_ms > 150)
                {
                    Audio::instance().play(SFXType::JETPACK);

                    auto& carrier_physics = registry.get<PhysicsComponent>(item.get_item_carrier_entity());
                    if (!carrier_physics.is_bottom_collision())
                    {
                        _fuel_activations = std::max<uint32_t>(0, static_cast<int>(_fuel_activations) - 1);

                        if (_fuel_activations == 0)
                        {
                            _cooldown_timer_ms = 1000;
                        }
                        else
                        {
                            carrier_physics.add_velocity(0, -0.18f);
                            prefabs::PoofParticle::create(position.x_center, position.y_center);
                        }
                        _activation_timer_ms = 0;
                    }
                }
            }
        }
    private:
        uint32_t _fuel_activations = max_fuel_activations;
        uint32_t _cooldown_timer_ms = 0;
        uint32_t _activation_timer_ms = 0;
        uint32_t _fuel_regeneration_timer_ms = 0;
    };
}

entt::entity prefabs::Jetpack::create()
{
    return create(0, 0);
}

entt::entity prefabs::Jetpack::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 1.0f;

    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    quad.frame_changed(CollectiblesSpritesheetFrames::JETPACK);

    PhysicsComponent physics(width, height);
    physics.set_friction(0.02f);

    ActivableComponent activable;
    activable.activate_combination = { InputEvent::JUMPING_PRESSED };

    ItemComponent item(ItemType::JETPACK, ItemApplication::ACTIVABLE, ItemSlot::BACK);
    item.set_carrying_offset({-0.4f, -0.2f});
    item.set_weight(7);

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_5_PROPS, CameraType::MODEL_VIEW_SPACE);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<ItemComponent>(entity, item);
    registry.emplace<ScriptingComponent>(entity, std::make_shared<JetpackScript>());
    registry.emplace<HorizontalOrientationComponent>(entity);
    registry.emplace<ActivableComponent>(entity, activable);

    return entity;
}
