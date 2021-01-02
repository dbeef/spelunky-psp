#pragma once

#include "MapTile.hpp"
#include "patterns/Subject.hpp"
#include "other/LootCollectedEvent.hpp"

#include <cstdint>

struct CollectibleComponent : public Subject<LootCollectedEvent>
{
    explicit CollectibleComponent(std::uint16_t value_dollars, LootCollectedEvent event_type)
        : value_dollars(value_dollars)
        , event_type(event_type) {}

    CollectibleComponent() = default;

    std::uint16_t value_dollars;
    LootCollectedEvent event_type;
};
