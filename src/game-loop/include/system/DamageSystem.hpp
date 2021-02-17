#pragma once

#include "System.hpp"

class DamageSystem final : public System
{
public:
    void update(std::uint32_t delta_time_ms) override;
private:
    static void update_falling_damage();
    static void update_tile_collision_damage();
    static void update_melee_damage();
    static void update_projectile_damage();
    static void update_jump_on_top_damage();
};
