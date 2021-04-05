#pragma once

#include "components/generic/ParticleEmitterComponent.hpp"
#include <cstdint>

class ParticleGenerator
{
public:

    ParticleGenerator particle_type(ParticleType type);
    ParticleGenerator quantity(std::uint16_t quantity);
    ParticleGenerator max_velocity(float x, float y);
    ParticleGenerator position(float x, float y);

    void finalize() const;
private:

    ParticleType _particle_type{};
    std::uint16_t _quantity = 0;

    struct
    {
        float x = 0.0f;
        float y = 0.0f;
    } _max_velocity;

    struct
    {
        float x = 0.0f;
        float y = 0.0f;
    } _position;
};
