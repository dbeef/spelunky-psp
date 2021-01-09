#include "system/ScriptingSystem.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include "components/specialized/PauseOverlayComponent.hpp"
#include "components/specialized/DeathOverlayComponent.hpp"
#include "components/specialized/LevelSummaryOverlayComponent.hpp"
#include "components/specialized/HudOverlayComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "EntityRegistry.hpp"

void ScriptingSystem::update(std::uint32_t delta_time_ms)
{
    auto& registry = EntityRegistry::instance().get_registry();

    registry.view<ScriptingComponent>().each([&registry, &delta_time_ms](entt::entity entity, ScriptingComponent& scripting_component){
        scripting_component.script->update(entity, delta_time_ms);
    });

    registry.view<MainDudeComponent>().each([&registry, &delta_time_ms](entt::entity entity, MainDudeComponent& dude){
        dude.update(delta_time_ms);
    });

    registry.view<PauseOverlayComponent>().each([&registry, &delta_time_ms](entt::entity entity, PauseOverlayComponent& pause){
        pause.update(registry);
    });

    registry.view<DeathOverlayComponent>().each([&registry, &delta_time_ms](entt::entity entity, DeathOverlayComponent& death){
        death.update();
    });

    // TODO: Rework to use ScriptingComponent:

    registry.view<LevelSummaryOverlayComponent>().each([&registry, &delta_time_ms](entt::entity entity, LevelSummaryOverlayComponent& level_summary){
        level_summary.update(delta_time_ms);
    });

    registry.view<HudOverlayComponent>().each([&registry, &delta_time_ms](entt::entity entity, HudOverlayComponent& hud){
        hud.update(delta_time_ms);
    });
}
