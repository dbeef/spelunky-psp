#pragma once

#include "System.hpp"

// TODO: ThieveryEvent in ShoppingSystem (Shopkeepers must subscribe to)
//       Non state-less Populator (saving shopkeepers, their respective items inside std::vector<Shop>)
//       Add _angry boolean to the Shopkeeper

class ShoppingSystem final : public System // Either keep track of all shopkeepers or emit ThieveryEvent (still need to save _robbed_shopkeeper somewhere)
{
public:
    void update(std::uint32_t delta_time_ms) override;
    bool& is_shopkeeper_robbed() { return _robbed_shopkeeper; }
private:
    bool _robbed_shopkeeper = false;
    void update_items_out_of_shop();
    static void update_dollar_sign_positions();
    static void update_transactions();
};
