#pragma once

#include "System.hpp"

class CollectibleSystem final : public System
{
public:
    void update(std::uint32_t delta_time_ms) override;
};
