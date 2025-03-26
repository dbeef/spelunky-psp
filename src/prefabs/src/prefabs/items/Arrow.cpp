#include "prefabs/items/Arrow.hpp"

#include "components/generic/ItemComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"
#include "components/damage/GiveProjectileDamageComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/NPCSpritesheetFrames.hpp"

#include <algorithm>

namespace
{
    class ArrowMutualDamageObserver final : public Observer<MutualDamage_t>
    {
    public:

        explicit ArrowMutualDamageObserver(entt::entity arrow) : _arrow(arrow) {}

        void on_notify(const MutualDamage_t *) override
        {
            auto& registry = EntityRegistry::instance().get_registry();

            auto& projectile_damage_component = registry.get<GiveProjectileDamageComponent>(_arrow);
            projectile_damage_component.remove_observer(this);

            auto& item = registry.get<ItemComponent>(_arrow);
            if (item.is_carried())
            {
                auto& item_carrier = item.get_item_carrier();
                item_carrier.put_down_active_item();
            }

            registry.destroy(_arrow);
        }

    private:
        const entt::entity _arrow;
    };

    class ArrowScript final : public ScriptBase
    {
    public:

        explicit ArrowScript(entt::entity arrow) : _damage_observer(arrow) {}

        ArrowMutualDamageObserver* get_observer()
        {
            return &_damage_observer;
        }

        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            const auto& item = registry.get<ItemComponent>(owner);
            const auto& physics = registry.get<PhysicsComponent>(owner);

            if (item.is_carried() || physics.get_y_velocity() == 0.0f)
            {
                set_frame(NPCSpritesheetFrames::ARROW, owner);
            }
            else
            {
                const auto angle = static_cast<int>(std::clamp(physics.get_y_velocity() / 0.04f, -4.0f, 4.0f));
                const auto base_frame = static_cast<std::size_t>(NPCSpritesheetFrames::ARROW_ANGLED_4);
                set_frame(static_cast<NPCSpritesheetFrames>(base_frame + angle), owner);
            }
        }
    private:
        ArrowMutualDamageObserver _damage_observer;
        NPCSpritesheetFrames _current = NPCSpritesheetFrames::ARROW;

        void set_frame(NPCSpritesheetFrames frame, entt::entity owner)
        {
            if (_current == frame)
            {
                return;
            }

            _current = frame;

            auto& registry = EntityRegistry::instance().get_registry();
            auto& quad = registry.get<QuadComponent>(owner);
            quad.frame_changed(_current);
        }
    };
}

entt::entity prefabs::Arrow::create()
{
    return create(0, 0);
}

entt::entity prefabs::Arrow::create(float pos_x_center, float pos_y_center, entt::entity throw_source)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 0.5f;
    const float height = 0.5f;

    QuadComponent quad(TextureType::NPC, width, height);
    quad.frame_changed(NPCSpritesheetFrames::ARROW);

    PhysicsComponent physics(width, height);
    physics.set_friction(0.02f);
    physics.set_bounciness(0.25f);

    ItemComponent item(ItemType::ARROW, ItemApplication::THROWABLE, ItemSlot::ACTIVE);
    item.set_weight(1.0f);

    auto arrow_script = std::make_shared<ArrowScript>(entity);
    ScriptingComponent script(arrow_script);

    GiveProjectileDamageComponent give_projectile_damage(2, throw_source);
    give_projectile_damage.set_mutual(true);
    give_projectile_damage.add_observer(reinterpret_cast<Observer<MutualDamage_t>*>(arrow_script->get_observer()));

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_3_ITEMS, CameraType::MODEL_VIEW_SPACE);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<ItemComponent>(entity, item);
    registry.emplace<ScriptingComponent>(entity, script);
    registry.emplace<HorizontalOrientationComponent>(entity);
    registry.emplace<GiveProjectileDamageComponent>(entity, give_projectile_damage);

    return entity;
}

