#pragma once

#include "System.hpp"

class DamageSystem final : public System
{
public:
    void update(std::uint32_t delta_time_ms) override;
private:
    static void update_falling_damage();
    static void update_melee_damage();
    static void update_projectile_damage();
};
