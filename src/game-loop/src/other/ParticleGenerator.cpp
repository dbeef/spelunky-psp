#include "components/generic/PhysicsComponent.hpp"
#include "prefabs/particles/BloodParticle.hpp"
#include "prefabs/particles/FlameParticle.hpp"
#include "other/ParticleGenerator.hpp"
#include "EntityRegistry.hpp"

#include <cstdlib>

namespace
{
    float randomize(float max)
    {
        const uint64_t i_max = max * 1000.0f;
        const float v = static_cast<float>(std::rand() % i_max) / 1000.0f;

        if (std::rand() % 2)
        {
            return -1 * v;
        }
        else
        {
            return v;
        }
    }
}

ParticleGenerator ParticleGenerator::position(float x, float y)
{
    _position.x = x;
    _position.y = y;
    return *this;
}

ParticleGenerator ParticleGenerator::max_velocity(float x, float y)
{
    _max_velocity.x = x;
    _max_velocity.y = y;
    return *this;
}

ParticleGenerator ParticleGenerator::quantity(uint16_t quantity)
{
    _quantity = quantity;
    return *this;
}

void ParticleGenerator::finalize() const
{
    auto& registry = EntityRegistry::instance().get_registry();

    for (std::uint16_t index = 0; index < _quantity; index++)
    {
        entt::entity particle;

        switch (_particle_type)
        {
            case ParticleType::BLOOD:
            {
                particle = prefabs::BloodParticle::create(_position.x, _position.y);
                break;
            }
            case ParticleType::FLAME:
            {
                particle = prefabs::FlameParticle::create(_position.x, _position.y);
                break;
            }
            default: assert(false);
        }

        auto& physics = registry.get<PhysicsComponent>(particle);

        const float v_x = randomize(_max_velocity.x);
        const float v_y = randomize(_max_velocity.y);

        physics.set_velocity(v_x, v_y);
    }
}

ParticleGenerator ParticleGenerator::particle_type(ParticleType type)
{
    _particle_type = type;
    return *this;
}
