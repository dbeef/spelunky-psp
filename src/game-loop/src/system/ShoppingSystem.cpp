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

    void remove_saleable(entt::entity item_id, bool do_create_transaction_particle)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& item = registry.get<ItemComponent>(item_id);
        auto& position = registry.get<PositionComponent>(item_id);
        auto& saleable = registry.get<SaleableComponent>(item_id);
        auto item_carrier_entity = item.is_carried() ? item.get_item_carrier_entity() : entt::null;

        if (item_carrier_entity != entt::null)
        {
            auto& item_carrier = registry.get<ItemCarrierComponent>(item_carrier_entity);
            item_carrier.put_down_active_item();
        }

        item.set_type(saleable.get_original_item_application());
        item.set_slot(saleable.get_original_item_slot());

        if (do_create_transaction_particle)
        {
            create_transaction_particle(item.get_type(), position.x_center, position.y_center - 0.75f);
        }

        if (registry.has<ScriptingComponent>(item_id))
        {
            auto& scripting_component = registry.get<ScriptingComponent>(item_id);
            scripting_component.script = saleable.get_original_script();
        }

        registry.remove<SaleableComponent>(item_id);

        if (item_carrier_entity != entt::null)
        {
            auto& item_carrier = registry.get<ItemCarrierComponent>(item_carrier_entity);
            item_carrier.pick_up_item(item_id, item_carrier_entity);
        }
    }

    void remove_all_saleables()
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto saleable_items = registry.view<SaleableComponent, ItemComponent, PositionComponent>();
        saleable_items.each([](entt::entity item_for_sale_entity, SaleableComponent&, ItemComponent&, PositionComponent&)
        {
            remove_saleable(item_for_sale_entity, false);
        });
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
                            wallet_script->notify(ShoppingTransactionEvent{get_successful_transaction_message(item.get_type())});
                            Audio::instance().play(SFXType::PICKUP);
                            remove_saleable(item_for_sale_entity, true);
                        }
                        else
                        {
                            // Transaction failed - not enough dollars - should prompt observers (i.e HUD) about it:
                            wallet_script->notify(ShoppingTransactionEvent{get_insufficient_funds_message()});
                            item_carrier.put_down_active_item();
                        }
                    }
                    else
                    {
                        // Transaction is possible - should prompt observers (i.e HUD) about it:
                        wallet_script->notify(ShoppingTransactionEvent{get_possible_transaction_message(item.get_type(), item_saleable.get_price_dollars())});
                    }
                }
            }
        }
    });
}

void ShoppingSystem::update_items_out_of_shop()
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto saleable_items = registry.view<ItemComponent, SaleableComponent, PositionComponent, PhysicsComponent>();

    saleable_items.each([&](entt::entity item_for_sale_entity,
                                 ItemComponent& item,
                                 SaleableComponent& item_saleable,
                                 PositionComponent& item_position,
                                 PhysicsComponent& item_physics)
    {
        if (_robbed)
        {
            return;
        }
        else if (!item_physics.is_collision(item_saleable.get_shop_zone(), item_saleable.get_shop_position(), item_position))
        {
            const entt::entity thief = item.is_carried() ? item.get_item_carrier_entity() : entt::null;
            notify(ThieveryEvent{thief});
            remove_all_saleables();
            _robbed = true;
        }
    });
}

void ShoppingSystem::on_notify(const ShopkeeperAssaultEvent *)
{
    if (_robbed)
    {
        return;
    }

    remove_all_saleables();
    _robbed = true;
}
