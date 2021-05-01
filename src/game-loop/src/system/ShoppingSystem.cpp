#include "system/ShoppingSystem.hpp"
#include "EntityRegistry.hpp"
#include "other/Inventory.hpp"
#include "audio/Audio.hpp"

#include "prefabs/items/Wallet.hpp"
#include "prefabs/particles/ItemCollectedParticle.hpp"
#include "prefabs/particles/RopeCollectedParticle.hpp"
#include "prefabs/particles/BombCollectedParticle.hpp"

#include "components/generic/SaleableComponent.hpp"
#include "components/generic/ActivableComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/ItemComponent.hpp"

#include <sstream>

namespace
{
    void create_transaction_particle(ItemType type, float pos_x_center, float pos_y_center)
    {
        switch (type)
        {
            case ItemType::BOMB_BAG: prefabs::BombCollectedParticle::create(pos_x_center, pos_y_center); break;
            case ItemType::ROPE_PILE: prefabs::RopeCollectedParticle::create(pos_x_center, pos_y_center); break;
            default: prefabs::ItemCollectedParticle::create(pos_x_center, pos_y_center); break;
        }
    }

    std::string item_name(ItemType type)
    {
#define ITEM_TYPE_STR(x) case ItemType::x: return #x;
        switch(type)
        {
            ITEM_TYPE_STR(MITT);
            ITEM_TYPE_STR(ARROW);
            ITEM_TYPE_STR(BOMB);
            ITEM_TYPE_STR(CAPE);
            ITEM_TYPE_STR(CHEST);
            ITEM_TYPE_STR(CRATE);
            ITEM_TYPE_STR(JAR);
            ITEM_TYPE_STR(JETPACK);
            ITEM_TYPE_STR(PISTOL);
            ITEM_TYPE_STR(ROCK);
            ITEM_TYPE_STR(ROPE);
            ITEM_TYPE_STR(SKULL);
            ITEM_TYPE_STR(WHIP);
            ITEM_TYPE_STR(WALLET);
            ITEM_TYPE_STR(SHOTGUN);
            ITEM_TYPE_STR(GLOVE);
            ITEM_TYPE_STR(COMPASS);
            case ItemType::BOMB_SPAWNER: return "BOMB SPAWNER";
            case ItemType::ROPE_SPAWNER: return "ROPE SPAWNER";
            case ItemType::SPIKE_SHOES: return "SPIKE SHOES";
            case ItemType::SPRING_SHOES: return "SPRING SHOES";
            case ItemType::ROPE_PILE: return "ROPE PILE";
            case ItemType::BOMB_BAG: return "BOMB BAG";
        }
#undef ITEM_TYPE_STR
        return "UNKNOWN";
    }

    std::string get_possible_transaction_message(ItemType item_type, int item_price_dollars)
    {
        std::stringstream ss;
        ss << item_name(item_type) << " FOR $" << item_price_dollars << " - PRESS "
           << Input::get_accept_transaction_binding_msg() << " TO BUY";
        return ss.str();
    }

    std::string get_insufficient_funds_message()
    {
        return "YOU HAVEN'T GOT ENOUGH MONEY!";
    }

    std::string get_successful_transaction_message(ItemType item_type)
    {
        std::stringstream ss;
        ss << "YOU GOT A " << item_name(item_type) << "!";
        return ss.str();
    }
}

void ShoppingSystem::update(std::uint32_t delta_time_ms)
{
    update_dollar_sign_positions();
    update_transactions();
    update_items_out_of_shop();
}

void ShoppingSystem::update_dollar_sign_positions()
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto saleable_items = registry.view<ItemComponent, SaleableComponent, PositionComponent>();

    saleable_items.each([](entt::entity item_for_sale_entity,
                           ItemComponent& item,
                           SaleableComponent& item_saleable,
                           PositionComponent& item_position)
    {
        if (item_saleable.get_dollar_sign_animation() != entt::null)
        {
            item_saleable.update_position(item_position);
        }
    });
}

void ShoppingSystem::update_transactions()
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto saleable_items = registry.view<ItemComponent, SaleableComponent, PositionComponent>();

    saleable_items.each([&registry](entt::entity item_for_sale_entity,
                                    ItemComponent& item,
                                    SaleableComponent& item_saleable,
                                    PositionComponent& item_position)
    {
        if (item.is_carried())
        {
            auto item_carrier_entity = item.get_item_carrier_entity();
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
                        // TODO: After rework to remove Inventory singleton, take the amount of dollars from Wallet instance.
                        // Finish transaction by removing dollar amount from wallet and removing SaleableComponent from
                        // item for sale:
                        if (Inventory::instance().get_dollars() >= item_saleable.get_price_dollars())
                        {
                            Inventory::instance().remove_dollars(item_saleable.get_price_dollars());
                            wallet_script->notify({get_successful_transaction_message(item.get_type())});

                            Audio::instance().play(SFXType::PICKUP);

                            item_carrier.put_down_active_item();

                            item.set_type(item_saleable.get_original_item_application());
                            item.set_slot(item_saleable.get_original_item_slot());

                            create_transaction_particle(item.get_type(), item_position.x_center, item_position.y_center - 0.75f);

                            if (registry.has<ScriptingComponent>(item_for_sale_entity))
                            {
                                auto& scripting_component = registry.get<ScriptingComponent>(item_for_sale_entity);
                                scripting_component.script = item_saleable.get_original_script();
                            }

                            registry.remove<SaleableComponent>(item_for_sale_entity);
                            item_carrier.pick_up_item(item_for_sale_entity, item_carrier_entity);
                        }
                        else
                        {
                            // Transaction failed - not enough dollars - should prompt observers (i.e HUD) about it:
                            wallet_script->notify({get_insufficient_funds_message()});
                            item_carrier.put_down_active_item();
                        }
                    }
                    else
                    {
                        // Transaction is possible - should prompt observers (i.e HUD) about it:
                        wallet_script->notify({get_possible_transaction_message(item.get_type(), item_saleable.get_price_dollars())});
                    }
                }
            }
        }
    });
}

void ShoppingSystem::update_items_out_of_shop()
{
    // TODO: Check if item with SaleableComponent is out of shop zone, if so, change state of shopkeeper to "angry"
    //       "COME BACK HERE, THIEF!" <- Prompt when item taken out of shop zone without purchase
}
