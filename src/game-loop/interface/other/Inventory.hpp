#pragma once

#include "patterns/Singleton.hpp"
#include "patterns/Subject.hpp"
#include "other/InventoryEvent.hpp"
#include "other/ItemType.hpp"

#include <cstdint>
#include <vector>

class Inventory : public Singleton<Inventory>, public Subject<InventoryEvent>
{
public:

    DELETE_COPY_MOVE_CONSTRUCTORS(Inventory)
    FRIEND_SINGLETON(Inventory)

    void set_starting_inventory();

    uint16_t get_hearts() const { return _hearts; }
    uint16_t get_dollars() const { return _dollars; }
    uint16_t get_ropes() const { return _ropes; }
    uint16_t get_bombs() const { return _bombs; }

    void add_dollars(uint16_t amount);
    void remove_hearts(uint16_t amount);
    
    void remove_bombs(uint16_t amount);
    void add_bombs(uint16_t amount);
    
    void remove_ropes(uint16_t amount);
    void add_ropes(uint16_t amount);

    const std::vector<ItemType>& get_items() const;
    void set_items(const std::vector<ItemType>& items);
    void clear_items();

private:

    Inventory();

    std::vector<ItemType> _items;

    int16_t _hearts;
    int16_t _ropes;
    int16_t _bombs;
    int16_t _dollars;
};
