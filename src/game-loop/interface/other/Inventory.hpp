#pragma once

#include "patterns/Singleton.hpp"
#include "patterns/Subject.hpp"
#include "other/InventoryEvent.hpp"
#include "other/ItemType.hpp"

#include <cstdint>
#include <vector>

// TODO: This middle-man in form of Inventory could be removed, and main-dude state inbetween
//       the levels could be stored in the GameLoop. HUD would be connected directly with HitpointComponent,
//       and items of type: RopeSpawner, BombSpawner, Wallet.

struct BombCountChangedEvent { int amount; };
struct RopeCountChangedEvent { int amount; };

class Inventory : public Singleton<Inventory>,
                  public Subject<InventoryEvent>,
                  public Observer<BombCountChangedEvent>,
                  public Observer<RopeCountChangedEvent>
{
public:

    DELETE_COPY_MOVE_CONSTRUCTORS(Inventory)
    FRIEND_SINGLETON(Inventory)

    void set_starting_inventory();

    int16_t get_hearts() const { return _hearts; }
    int16_t get_ropes() const { return _ropes; }
    int16_t get_bombs() const { return _bombs; }
    int32_t get_dollars() const { return _dollars; }

    void remove_dollars(uint16_t amount);
    void add_dollars(uint16_t amount);
    void remove_hearts(uint16_t amount);
    void add_hearts(uint16_t amount);

    void set_bombs(uint16_t amount);
    void set_ropes(uint16_t amount);

    const std::vector<ItemType>& get_items() const;
    void set_items(const std::vector<ItemType>& items);
    void clear_items();

    void on_notify(const BombCountChangedEvent*) override;
    void on_notify(const RopeCountChangedEvent*) override;

private:

    Inventory();

    std::vector<ItemType> _items;

    int16_t _hearts;
    int16_t _ropes;
    int16_t _bombs;
    int32_t _dollars;
};
