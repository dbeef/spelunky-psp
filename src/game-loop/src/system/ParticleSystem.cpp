#include "system/ParticleSystem.hpp"
#include "components/generic/ParticleEmitterComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "EntityRegistry.hpp"
#include "prefabs/particles/ItemCollectedParticle.hpp"
#include "prefabs/particles/BloodTrailParticle.hpp"
#include "prefabs/particles/FlameTrailParticle.hpp"
#include "prefabs/particles/FlareSparkParticle.hpp"

#include <cassert>

void ParticleSystem::update(std::uint32_t delta_time_ms)
{
    auto &registry = EntityRegistry::instance().get_registry();
    registry.view<ParticleEmitterComponent>().each([&delta_time_ms, &registry](entt::entity e, ParticleEmitterComponent& c){
        c.time_since_last_emission_ms += delta_time_ms;
        if (c.time_since_last_emission_ms > c.interval_ms)
        {
            c.time_since_last_emission_ms -= c.interval_ms;
            auto& position = registry.get<PositionComponent>(e);

            switch (c.particle_type)
            {
                case ParticleType::ITEM_COLLECTED:
                {
                    prefabs::ItemCollectedParticle::create(position.x_center, position.y_center);
                    break;
                }
                case ParticleType::BLOOD_TRAIL:
                {
                    prefabs::BloodTrailParticle::create(position.x_center, position.y_center);
                    break;
                }
                case ParticleType::FLAME_TRAIL:
                {
                    prefabs::FlameTrailParticle::create(position.x_center, position.y_center);
                    break;
                }
                case ParticleType::FLARE_SPARK:
                {
                    prefabs::FlareSparkParticle::create(position.x_center, position.y_center);
                    break;
                }
                case ParticleType::ROPE_COLLECTED:break;
                case ParticleType::BOMB_COLLECTED:break;
                default: assert(false);
            }
        }
    });
}
