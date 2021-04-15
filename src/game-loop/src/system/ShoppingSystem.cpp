#include "system/ShoppingSystem.hpp"
#include "EntityRegistry.hpp"
#include "other/Inventory.hpp"

#include "components/generic/SaleableComponent.hpp"
#include "components/generic/ActivableComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/ItemComponent.hpp"

#include "prefabs/items/Wallet.hpp"

#include <sstream>

namespace
{
    std::string item_name(ItemType type)
    {
        switch(type)
        {
            case ItemType::ARROW:break;
            case ItemType::BOMB:break;
            case ItemType::CAPE:break;
            case ItemType::CHEST:break;
            case ItemType::CRATE:break;
            case ItemType::JAR:break;
            case ItemType::JETPACK:break;
            case ItemType::PISTOL:break;
            case ItemType::ROCK:break;
            case ItemType::ROPE:break;
            case ItemType::SHOTGUN: return "SHOTGUN";
            case ItemType::SKULL:break;
            case ItemType::WHIP:break;
            case ItemType::BOMB_SPAWNER:break;
            case ItemType::ROPE_SPAWNER:break;
            case ItemType::WALLET:break;
        }

        return "UNKNOWN";
    }

    // std::to_string is missing in PSP's libc++.
    std::string to_string(uint32_t value)
    {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }
}

void ShoppingSystem::update(std::uint32_t delta_time_ms)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto saleable_items = registry.view<ItemComponent, SaleableComponent, PositionComponent>();

    saleable_items.each([&registry](entt::entity item_for_sale_entity,
                                         ItemComponent& item,
                                         SaleableComponent& item_saleable,
                                         PositionComponent& item_position)
    {
        // Update dollar sign positions:

        if (item_saleable.get_dollar_sign_animation() != entt::null)
        {
            item_saleable.update_position(item_position);
        }

        // Check if item is in hands:

        if (item.is_carried())
        {
            auto& item_carrier = registry.get<ItemCarrierComponent>(item.get_item_carrier_entity());
            const auto& items = item_carrier.get_items();
            const auto carried_item_entities = item_carrier.get_item_entities();

            for (const auto& carried_item : carried_item_entities)
            {
                if (carried_item == entt::null)
                {
                    continue;
                }

                auto &carried_item_component = registry.get<ItemComponent>(carried_item);
                if (carried_item_component.get_type() == ItemType::WALLET)
                {
                    auto& wallet_entity = carried_item;

                    auto& wallet_scripting_component = registry.get<ScriptingComponent>(wallet_entity);
                    auto* wallet_script = reinterpret_cast<prefabs::WalletScript*>(wallet_scripting_component.script.get());
                    auto& activable = registry.get<ActivableComponent>(wallet_entity);

                    if (activable.activated)
                    {
                        // Finish transaction by removing dollar amount from wallet and removing SaleableComponent from
                        // item for sale:
                        if (Inventory::instance().get_dollars() >= item_saleable.get_price_dollars())
                        {
                            Inventory::instance().remove_dollars(item_saleable.get_price_dollars());
                            wallet_script->notify({"BOUGHT A " + item_name(item.get_type())});
                            registry.remove<SaleableComponent>(item_for_sale_entity);
                        }
                        else
                        {
                            // Transaction failed - not enough dollars - should prompt observers (i.e HUD) about it:
                            wallet_script->notify({"INSUFFICIENT FUNDS - NEED " + to_string(item_saleable.get_price_dollars()) + "$"});
                            item_carrier.put_down_active_item();
                        }
                    }
                    else
                    {
                        // Transaction is possible - should prompt observers (i.e HUD) about it:
                        wallet_script->notify({item_name(item.get_type()) + " FOR " + to_string(item_saleable.get_price_dollars()) + "$"});
                    }
                }
            }
        }
    });
}
