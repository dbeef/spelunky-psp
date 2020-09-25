#pragma once

#include "GameEntity.hpp"

#include "components/PhysicsComponent.hpp"
#include "components/QuadComponent.hpp"
#include "patterns/Subject.hpp"
#include "other/LootCollectedEvent.hpp"

class TripleGoldBar : public GameEntity, public Subject<LootCollectedEvent>
{
public:

    TripleGoldBar(float x_center, float y_center);
    void update(uint32_t delta_time_ms) override;

private:
    PhysicsComponent _physics;
    QuadComponent _quad;
};
