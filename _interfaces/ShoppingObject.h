//
// Created by xdbeef on 17.06.18.
//

#ifndef SPELUNKYDS_SHOPPING_OBJECT_H
#define SPELUNKYDS_SHOPPING_OBJECT_H

#include "../creatures/_BaseCreature.h"
#include "../decorations/ShoppingIcon.h"
#include "../items/_BaseItem.h"

class ShoppingObject {

public:

    ShoppingObject(u16 cost, const char *name) : _cost(cost), _name(name) {
        // do nothing
    }

    bool shopping_transaction(BaseItem *m);

    bool shopping_transaction(BaseCreature *m);

    void console_display_name_cost();

    void init_anim_icon();

    void update_anim_icon(int x, int y, int carrier_width);

    bool _bought{};
    u16 _cost;
    const char *_name;
    ShoppingIcon *_shopping_icon{};

};


#endif //SPELUNKYDS_SHOPPING_OBJECT_H
