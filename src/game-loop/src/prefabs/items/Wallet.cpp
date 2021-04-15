#include "prefabs/items/Wallet.hpp"

#include "components/generic/ActivableComponent.hpp"
#include "components/generic/ItemComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"

#include "EntityRegistry.hpp"

entt::entity prefabs::Wallet::create()
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    ActivableComponent activable;
    activable.activate_combination = { InputEvent::ACCEPT_TRANSACTION,  InputEvent::ACCEPT_TRANSACTION_PRESSED };

    registry.emplace<ItemComponent>(entity, ItemType::WALLET, ItemApplication::PASSIVE, ItemSlot::OTHER);
    registry.emplace<ScriptingComponent>(entity, std::make_shared<WalletScript>());
    registry.emplace<ActivableComponent>(entity, activable);

    return entity;
}
