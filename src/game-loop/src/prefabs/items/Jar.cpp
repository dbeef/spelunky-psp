#include "prefabs/items/Jar.hpp"
#include "prefabs/particles/SmokePuffParticle.hpp"
#include "prefabs/particles/RubbleSmallParticle.hpp"
#include "prefabs/collectibles/BigGem.hpp"
#include "prefabs/collectibles/GoldChunk.hpp"
#include "prefabs/collectibles/GoldNugget.hpp"

#include "components/damage/GiveProjectileDamageComponent.hpp"
#include "components/generic/ItemComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

#include <cmath>

namespace
{
    enum class JarLootType : uint8_t
    {
        BIG_GEM,
        GOLD_CHUNK,
        GOLD_NUGGET,
        _SIZE
    };

    JarLootType get_random_loot()
    {
        return static_cast<JarLootType>(std::rand() % static_cast<int>(JarLootType::_SIZE));
    }

    class JarScript final : public ScriptBase
    {
    public:

        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& physics = registry.get<PhysicsComponent>(owner);

            if (physics.is_left_collision() || physics.is_right_collision() ||
                physics.is_upper_collision() || physics.is_bottom_collision())
            {
                if (std::fabs(_last_x_speed) > 0.15f || std::fabs(_last_y_speed) > 0.13f)
                {
                    auto& position = registry.get<PositionComponent>(owner);
                    prefabs::SmokePuffParticle::create(position.x_center, position.y_center);

                    switch (get_random_loot())
                    {
                        case JarLootType::BIG_GEM: prefabs::BigGem::create(position.x_center, position.y_center); break;
                        case JarLootType::GOLD_CHUNK: prefabs::GoldChunk::create(position.x_center, position.y_center); break;
                        case JarLootType::GOLD_NUGGET: prefabs::GoldNugget::create(position.x_center, position.y_center); break;
                    }

                    for (int index = 0; index < 5; ++index)
                    {
                        auto rubble = prefabs::RubbleSmallParticle::create(position.x_center, position.y_center);
                        auto& rubble_physics = registry.get<PhysicsComponent>(rubble);

                        float rubble_velocity_x = 0;
                        float rubble_velocity_y = 0;

                        rubble_velocity_x = static_cast<float>(std::rand() % 10) / 100.0f;
                        rubble_velocity_y = static_cast<float>(std::rand() % 10) / 100.0f;

                        rubble_physics.set_x_velocity(rubble_velocity_x);
                        rubble_physics.set_y_velocity(rubble_velocity_y);

                    }

                    registry.destroy(owner);
                }
            }

            _last_x_speed = physics.get_x_velocity();
            _last_y_speed = physics.get_y_velocity();
        }

    private:
        float _last_x_speed = 0;
        float _last_y_speed = 0;
    };
}

entt::entity prefabs::Jar::create()
{
    return create(0, 0);
}

entt::entity prefabs::Jar::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 1.0f;

    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    quad.frame_changed(CollectiblesSpritesheetFrames::JAR);

    PhysicsComponent physics(width, height);
    physics.set_friction(0.02f);

    ItemComponent item(ItemType::THROWABLE, ItemSlot::ACTIVE);
    item.set_weight(2);
    item.set_carrying_offset({0.0f, -1.0f/8.0f});

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_2_ITEMS, CameraType::MODEL_VIEW_SPACE);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<ItemComponent>(entity, item);
    registry.emplace<ScriptingComponent>(entity, std::make_shared<JarScript>());
    registry.emplace<HorizontalOrientationComponent>(entity);
    registry.emplace<GiveProjectileDamageComponent>(entity, 1);

    return entity;
}
