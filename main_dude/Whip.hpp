//
// Created by xdbeef on 07.05.18.
//

#ifndef SPELUNKYDS_WHIP_H
#define SPELUNKYDS_WHIP_H

//FIXME used in collisions
#define WHIP_WIDTH 12

#include "../creatures/_BaseCreature.h"
#include "../../memory/SpriteInfo.h"

//http://spelunky.wikia.com/wiki/Whip
class Whip : public BaseCreature {

    static constexpr u8 whip_sprite_width = 16;
    static constexpr u8 whip_sprite_height = 16;
    static constexpr u16 whip_physical_width = 16;
    static constexpr u16 whip_physical_height = 16;
    static constexpr SpritesheetType whip_spritesheet_type = SpritesheetType::SPIKES_COLLECTIBLES;

public:

    Whip(int x, int y) : BaseCreature(
            x,
            y,
            whip_sprite_width,
            whip_sprite_height,
            whip_spritesheet_type,
            whip_physical_width,
            whip_physical_height,
            CreatureType::WHIP
    ) {
        init_sprites();
    }

    // Base creature overrides

    void update_creature_specific() override;

    void introduce_yourself() override { printf("WHIP\n"); };

    void apply_dmg(int dmg_to_apply) override {};

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    // Whip's position depends on main dude's position. No collision checking.
    bool can_update_collidable() const override { return false; }
    bool can_apply_friction() const override { return false; }
    bool can_apply_gravity() const override { return false; }

    // Other, creature specific

    void assign_pre_whip_sprite();

    void assign_whip_sprite();

    void hide();

    SpriteInfo *_main_sprite_info{};
    SpriteInfo *_sub_sprite_info{};
    u8 *_frame_gfx{};
    double _whiping_timer{};

private:

    //Updates sprites' graphics according to current animation frame index.
    void match_animation();
};


#endif //SPELUNKYDS_WHIP_H
