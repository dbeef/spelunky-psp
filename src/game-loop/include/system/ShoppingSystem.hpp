#pragma once

#include "patterns/Subject.hpp"
#include "System.hpp"

struct ThieveryEvent
{
    entt::entity thief = entt::null;
};

struct ShopkeeperAssaultEvent
{
    entt::entity attacker = entt::null;
};

class ShoppingSystem final : public System, public Subject<ThieveryEvent>, public Observer<ShopkeeperAssaultEvent>
{
public:
    void update(std::uint32_t delta_time_ms) override;
    void on_notify(const ShopkeeperAssaultEvent*);
    bool is_robbed() const { return _robbed; }
private:
    bool _robbed = false;
    void update_items_out_of_shop();
    static void update_dollar_sign_positions();
    static void update_transactions();
};
