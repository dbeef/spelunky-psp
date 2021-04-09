#include "prefabs/items/Whip.hpp"

#include "components/generic/ItemComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"
#include "components/generic/ActivableComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include "components/damage/GiveMeleeDamageComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

namespace
{
    const float width = 1.0f;
    const float height = 1.0f;

    const uint32_t total_duration_ms = 550;
    const uint32_t pre_whip_frame_duration_ms = 100;

    class WhipScript final : public ScriptBase
    {
    public:
        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& item = registry.get<ItemComponent>(owner);
            auto& activable = registry.get<ActivableComponent>(owner);

            _cooldown_timer_ms += delta_time_ms;

            auto& carrier = item.get_item_carrier();
            if (carrier.has_active_item())
            {
                // Don't show nor activate when there's already an active item:
                _cooldown_timer_ms = total_duration_ms;
                hide(owner);
                return;
            }

            if (_cooldown_timer_ms >= total_duration_ms)
            {
                hide(owner);
            }
            else if (_cooldown_timer_ms >= pre_whip_frame_duration_ms &&
                     _current_frame != CollectiblesSpritesheetFrames::WHIP)
            {
                _current_frame = CollectiblesSpritesheetFrames::WHIP;

                item.set_carrying_offset({0.75f, 0.1f});

                auto &quad = registry.get<QuadComponent>(owner);
                quad.frame_changed(_current_frame);
            }

            if (activable.activated && _cooldown_timer_ms >= total_duration_ms)
            {
                // Just activated item:

                _cooldown_timer_ms = 0;

                show(owner);

                item.set_carrying_offset({-1.25f, -0.05f});

                if (registry.has<MainDudeComponent>(item.get_item_carrier_entity()))
                {
                    auto& main_dude = registry.get<MainDudeComponent>(item.get_item_carrier_entity());
                    main_dude.enter_throwing_state();
                }

                activable.activated = false;
            }
        }

    private:
        CollectiblesSpritesheetFrames _current_frame;
        uint32_t _cooldown_timer_ms = total_duration_ms;
        
        static void hide(entt::entity owner)
        {
            auto& registry = EntityRegistry::instance().get_registry();
            
            if (registry.has<QuadComponent>(owner))
            {
                registry.remove<QuadComponent>(owner);
            }
            if (registry.has<AnimationComponent>(owner))
            {
                registry.remove<AnimationComponent>(owner);
            }
            if (registry.has<MeshComponent>(owner))
            {
                registry.remove<MeshComponent>(owner);
            }
            if (registry.has<GiveMeleeDamageComponent>(owner))
            {
                registry.remove<GiveMeleeDamageComponent>(owner);
            }
        }
        
        void show(entt::entity owner)
        {
            auto& registry = EntityRegistry::instance().get_registry();

            MeshComponent mesh;
            mesh.rendering_layer = RenderingLayer::LAYER_3_ITEMS;
            mesh.camera_type = CameraType::MODEL_VIEW_SPACE;

            QuadComponent quad(TextureType::COLLECTIBLES, width, height);
            _current_frame = CollectiblesSpritesheetFrames::PRE_WHIP;

            quad.frame_changed(_current_frame);

            registry.emplace<MeshComponent>(owner, mesh);
            registry.emplace<QuadComponent>(owner, quad);
            registry.emplace<GiveMeleeDamageComponent>(owner, 1);
        }
    };
}

entt::entity prefabs::Whip::create()
{
    return create(0, 0);
}

entt::entity prefabs::Whip::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    ActivableComponent activable;
    activable.activate_combination = { InputEvent::THROWING_PRESSED };

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<PhysicsComponent>(entity, width, height);
    registry.emplace<ItemComponent>(entity, ItemType::WHIP, ItemApplication::ACTIVABLE, ItemSlot::OTHER);
    registry.emplace<ScriptingComponent>(entity, std::make_shared<WhipScript>());
    registry.emplace<HorizontalOrientationComponent>(entity);
    registry.emplace<ActivableComponent>(entity, activable);

    return entity;
}
