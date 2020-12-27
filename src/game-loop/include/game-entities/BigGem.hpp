#pragma once

#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"
#include "GameEntity.hpp"

#include "components/PhysicsComponent.hpp"
#include "components/QuadComponent.hpp"
#include "patterns/Subject.hpp"
#include "other/LootCollectedEvent.hpp"

class BigGem : public GameEntity, public Subject<LootCollectedEvent>
{
public:
    BigGem(float x_center, float y_center);
    void update(World* world, uint32_t delta_time_ms) override;
private:
    CollectiblesSpritesheetFrames _gem_type;
};
