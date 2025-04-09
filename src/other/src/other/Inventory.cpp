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
    _items.clear();
}

void Inventory::add_hearts(uint16_t amount)
{
    _hearts += amount;
    notify(InventoryEvent::HEARTS_COUNT_CHANGED);
}

void Inventory::remove_hearts(uint16_t amount)
{
    _hearts = std::max<int16_t>(0, _hearts - amount);
    notify(InventoryEvent::HEARTS_COUNT_CHANGED);
}

void Inventory::remove_dollars(uint16_t amount)
{
    _dollars -= amount;
    notify(InventoryEvent::DOLLARS_COUNT_CHANGED);
}

void Inventory::add_dollars(uint16_t amount)
{
    _dollars += amount;
    notify(InventoryEvent::DOLLARS_COUNT_CHANGED);
}

void Inventory::set_ropes(uint16_t amount)
{
    _ropes = amount;
    notify(InventoryEvent::ROPES_COUNT_CHANGED);
}

void Inventory::set_bombs(uint16_t amount)
{
    _bombs = amount;
    notify(InventoryEvent::BOMBS_COUNT_CHANGED);
}

const std::vector<ItemType> &Inventory::get_items() const
{
    return _items;
}

void Inventory::set_items(const std::vector<ItemType> &items)
{
    _items = items;
}

void Inventory::clear_items()
{
    _items.clear();
}

void Inventory::on_notify(const RopeCountChangedEvent* event)
{
    set_ropes(event->amount);
}

void Inventory::on_notify(const BombCountChangedEvent* event)
{
    set_bombs(event->amount);
}
