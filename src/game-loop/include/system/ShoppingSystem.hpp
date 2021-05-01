#pragma once

#include "System.hpp"

class ShoppingSystem final : public System
{
public:
    void update(std::uint32_t delta_time_ms) override;
private:
    static void update_dollar_sign_positions();
    static void update_transactions();
    static void update_items_out_of_shop();
};
