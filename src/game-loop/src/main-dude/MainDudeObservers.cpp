#include "main-dude/MainDudeObservers.hpp"
#include "prefabs/particles/PoofParticle.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include "components/generic/BlinkingComponent.hpp"
#include "components/damage/GiveNpcTouchDamageComponent.hpp"
#include "components/damage/GiveJumpOnTopDamage.hpp"
#include "components/generic/ParticleEmitterComponent.hpp"
#include "EntityRegistry.hpp"
#include "other/Inventory.hpp"
#include "other/ParticleGenerator.hpp"
#include "audio/Audio.hpp"

void MainDudeClimbingObserver::on_notify(const ClimbingEvent *event)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& main_dude_component = registry.get<MainDudeComponent>(_main_dude);

    _last_event = *event;

    switch (_last_event.event_type)
    {
        case ClimbingEventType::STARTED_CLIMBING_LADDER:
        case ClimbingEventType::STARTED_CLIMBING_ROPE:
        {
            main_dude_component.enter_climbing_state();
            break;
        }
    }
}

void MainDudeProjectileDamageObserver::on_notify(const ProjectileDamage_t *event)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& main_dude_component = registry.get<MainDudeComponent>(_main_dude);
    auto& position = registry.get<PositionComponent>(_main_dude);

    Inventory::instance().remove_hearts(*event);
    Audio::instance().play(SFXType::MAIN_DUDE_HURT);

    if (Inventory::instance().get_hearts() > 0)
    {
        main_dude_component.enter_stunned_state();
        ParticleGenerator().particle_type(ParticleType::BLOOD)
                .position(position.x_center, position.y_center)
                .max_velocity(0.25f, 0.25f)
                .quantity(2)
                .finalize();
    }
}

void MainDudeSpikesDamageObserver::on_notify(const SpikesDamageEvent *event)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& hitpoints = registry.get<HitpointComponent>(_main_dude);

    Inventory::instance().remove_hearts(hitpoints.get_hitpoints());
    Audio::instance().play(SFXType::MAIN_DUDE_HURT);
}

void MainDudeExplosionDamageObserver::on_notify(const ExplosionDamageTakenEvent *event)
{
    auto& registry = EntityRegistry::instance().get_registry();

    if (registry.has<ParticleEmitterComponent>(_main_dude))
    {
        return;
    }

    ParticleEmitterComponent particle_emitter;

    particle_emitter.interval_ms = 250;
    particle_emitter.particle_type = ParticleType::FLAME_TRAIL;
    particle_emitter.time_since_last_emission_ms = 0;

    registry.emplace<ParticleEmitterComponent>(_main_dude, particle_emitter);

    Audio::instance().play(SFXType::MAIN_DUDE_HURT);
}

void MainDudeNpcDamageObserver::on_notify(const NpcDamage_t *event)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& main_dude_component = registry.get<MainDudeComponent>(_main_dude);

    Inventory::instance().remove_hearts(*event);
    Audio::instance().play(SFXType::MAIN_DUDE_HURT);

    if (Inventory::instance().get_hearts() == 0)
    {
        main_dude_component.enter_dead_state();
    }
    else if (!registry.has<BlinkingComponent>(_main_dude))
    {
        const int interval_ms = 50;
        const int total_time = GiveNpcTouchDamageComponent::TOP_COOLDOWN_MS;
        const int remove_on_done = true;

        registry.emplace<BlinkingComponent>(_main_dude, interval_ms, total_time, remove_on_done);
    }
    else
    {
        auto& blinking_component = registry.get<BlinkingComponent>(_main_dude);
        blinking_component.reset_timer();
    }
}

void MainDudeDeathObserver::on_notify(const DeathEvent *event)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& main_dude_component = registry.get<MainDudeComponent>(_main_dude);
    auto& position = registry.get<PositionComponent>(_main_dude);

    if (registry.has<GiveJumpOnTopDamageComponent>(_main_dude)) registry.remove<GiveJumpOnTopDamageComponent>(_main_dude);

    main_dude_component.notify(MainDudeEvent::DIED);
    main_dude_component.enter_dead_state();

    ParticleGenerator().particle_type(ParticleType::BLOOD)
            .position(position.x_center, position.y_center)
            .max_velocity(0.25f, 0.25f)
            .quantity(2)
            .finalize();
}

void MainDudeFallObserver::on_notify(const FallDamage_t *event)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& main_dude_component = registry.get<MainDudeComponent>(_main_dude);
    auto& position = registry.get<PositionComponent>(_main_dude);

    Audio::instance().play(SFXType::MAIN_DUDE_HURT);
    prefabs::PoofParticle::create(position.x_center - (MapTile::PHYSICAL_WIDTH / 2.0f), position.y_center);
    prefabs::PoofParticle::create(position.x_center + (MapTile::PHYSICAL_WIDTH / 2.0f), position.y_center);
    Inventory::instance().remove_hearts(*event);

    if (Inventory::instance().get_hearts() == 0)
    {
        main_dude_component.enter_dead_state();
    }
    else
    {
        main_dude_component.enter_stunned_state();
    }
}
