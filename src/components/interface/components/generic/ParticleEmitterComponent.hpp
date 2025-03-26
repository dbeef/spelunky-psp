#pragma once

#include <cstdint>

enum class ParticleType
{
    ITEM_COLLECTED,
    ROPE_COLLECTED,
    BOMB_COLLECTED,
    BLOOD_TRAIL,
    FLAME_TRAIL,
    BLOOD,
    FLAME,
    FLARE_SPARK,
};

struct ParticleEmitterComponent
{
    int interval_ms;
    int time_since_last_emission_ms;
    ParticleType particle_type;
};
