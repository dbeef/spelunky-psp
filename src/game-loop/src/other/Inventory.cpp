#include "other/Inventory.hpp"

REGISTER_SINGLETON_INSTANCE(Inventory)

Inventory::Inventory()
{
    set_starting_inventory();
}

void Inventory::set_starting_inventory()
{
    _hearts = 4;
    _bombs = 4;
    _ropes = 4;
    _dollars = 0;
}

void Inventory::remove_hearts(uint16_t amount)
{
    _hearts = std::max<int16_t>(0, _hearts - amount);
    notify(InventoryEvent::HEARTS_COUNT_CHANGED);
}

void Inventory::add_dollars(uint16_t amount)
{
    _dollars += amount;
    notify(InventoryEvent::DOLLARS_COUNT_CHANGED);
}
