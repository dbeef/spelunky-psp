#pragma once

#include "GameEntity.hpp"

#include "components/PhysicsComponent.hpp"
#include "components/QuadComponent.hpp"

class SingleGoldBar : public GameEntity
{
public:

    SingleGoldBar(float x_center, float y_center);
    void update(uint32_t delta_time_ms) override;

private:
    PhysicsComponent _physics;
    QuadComponent _quad;
};
