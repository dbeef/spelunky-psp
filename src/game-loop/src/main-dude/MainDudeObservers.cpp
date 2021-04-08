#include "main-dude/MainDudeObservers.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include "components/generic/BlinkingComponent.hpp"
#include "components/damage/GiveNpcTouchDamageComponent.hpp"
#include "EntityRegistry.hpp"
#include "other/Inventory.hpp"

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

void MainDudeNpcDamageObserver::on_notify(const NpcDamage_t *event)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& main_dude_component = registry.get<MainDudeComponent>(_main_dude);

    Inventory::instance().remove_hearts(*event);

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

    main_dude_component.notify(MainDudeEvent::DIED);
    main_dude_component.enter_dead_state();
}

void MainDudeFallObserver::on_notify(const FallDamage_t *event)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& main_dude_component = registry.get<MainDudeComponent>(_main_dude);

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
