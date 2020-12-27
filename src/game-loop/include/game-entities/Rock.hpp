#pragma once

#include "GameEntity.hpp"

#include "components/PhysicsComponent.hpp"
#include "components/QuadComponent.hpp"

class Rock : public GameEntity
{
public:
    Rock(float x_center, float y_center);
    void update(World* world, uint32_t delta_time_ms) override;
};
