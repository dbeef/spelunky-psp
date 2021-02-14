#pragma once

#include "System.hpp"

class DamageSystem final : public System
{
public:
    void update(std::uint32_t delta_time_ms) override;
private:
    void update_falling_damage();
};
