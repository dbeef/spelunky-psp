#include "system/CollectibleSystem.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include "components/generic/CollectibleComponent.hpp"
#include "EntityRegistry.hpp"
#include "audio/Audio.hpp"
#include "audio/SFXType.hpp"
#include "other/Inventory.hpp"

void CollectibleSystem::update(std::uint32_t delta_time_ms)
{
    auto &registry = EntityRegistry::instance().get_registry();

    auto dudes = registry.view<MainDudeComponent>();
    assert(dudes.size() == 1);
    auto dude = dudes.front();

    auto &dude_physics = registry.get<PhysicsComponent>(dude);
    auto &dude_position = registry.get<PositionComponent>(dude);

    // FIXME: Technically the only thing that can gather collectibles is main dude, so counting all collectibles
    //        into Inventory::instance() is not wrong, however - in case I wanted to split it up, i.e potentially multiplayer
    //        with multiple main-dudes, this would need to be re-written so to each collectible-gatherer would have its
    //        own dollar counter.

    auto check_collisions =
            [&registry, &dude_physics, &dude_position]
                    (entt::entity entity, CollectibleComponent &collectible, PositionComponent &position,
                     PhysicsComponent &physics)
            {
                if (dude_physics.is_collision(physics, position, dude_position))
                {
                    switch (collectible.event_type)
                    {
                        case LootCollectedEvent::SINGLE_GOLD_BAR:
                        case LootCollectedEvent::TRIPLE_GOLD_BAR:
                        case LootCollectedEvent::GOLD_NUGGET:
                        case LootCollectedEvent::GOLD_CHUNK: Audio::instance().play(SFXType::COIN); break;
                        case LootCollectedEvent::BIG_GEM:
                        case LootCollectedEvent::SMALL_GEM: Audio::instance().play(SFXType::GEM); break;
                    }

                    Inventory::instance().add_dollars(collectible.value_dollars);
                    collectible.notify(collectible.event_type);
                    registry.destroy(entity);
                }
            };

    registry.view<CollectibleComponent, PositionComponent, PhysicsComponent>().each(check_collisions);
}

