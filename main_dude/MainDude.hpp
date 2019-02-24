//
// Created by xdbeef on 24.02.18.
//
//https://www.quora.com/Are-primitives-default-initialized-in-C++11

#ifndef SPELUNKYDS_MAINDUDE_H
#define SPELUNKYDS_MAINDUDE_H

//TODO Make it constexpr
#define MAIN_DUDE_MAX_X_SPEED 2
#define MAIN_DUDE_MAX_X_SPEED_CRAWLING 1.5f
#define MAIN_DUDE_MAX_X_SPEED_RUNNING 3
#define MAIN_DUDE_MAX_Y_SPEED 3.3f
#define MAIN_DUDE_MAX_Y_SPEED_USING_CAPE 1.7f
#define MAIN_DUDE_JUMP_SPEED 2.55f
#define MAIN_DUDE_MIN_HANGING_TIME 100
#define MAIN_DUDE_STUN_TIME 2000
#define MAIN_DUDE_STUN_FALLING_TIME 900
#define MAIN_DUDE_PUSHING_TIME 500
#define MAIN_DUDE_DAMAGE_PROTECTION_TIME 1500
#define MAIN_DUDE_FRAMES_PER_ANIMATION 6
#define MAIN_DUDE_HITPOINTS 4
#define MAIN_DUDE_SPRITESHEET_ROW_WIDTH 6
#define MAIN_DUDE_X_SPEED_DELTA_TIME_MS 2
#define MAIN_DUDE_X_SPEED_DELTA_VALUE 2
#define MAIN_DUDE_FRICTION_DELTA_SPEED 1

#include <nds.h>
#include <vector>

#include "../_common/Orientation.hpp"
#include "../creatures/_BaseCreature.h"
#include "../items/_BaseItem.h"
#include "Whip.hpp"
#include "../_interfaces/IPickupable.h"

//http://spelunky.wikia.com/wiki/Spelunky_Guy
class MainDude : public BaseCreature {

public:

    static constexpr u8 main_dude_sprite_width = 16;
    static constexpr u8 main_dude_sprite_height = 16;
    static constexpr u16 main_dude_physical_width = 16;
    static constexpr u16 main_dude_physical_height = 16;
    static constexpr SpritesheetType main_dude_spritesheet_type = SpritesheetType::MAIN_DUDE;

    static constexpr u16 main_dude_pos_update_delta_walking_running = 15;
    static constexpr u16 main_dude_pos_update_delta_crawling = 30;
    static constexpr float main_dude_max_x_speed_walking = 1.50f;
    static constexpr float main_dude_max_x_speed_running = 3.0f;
    static constexpr float main_dude_max_x_crawling = 0.55f;

    MainDude(int x, int y) : BaseCreature(
            x,
            y,
            main_dude_sprite_width,
            main_dude_sprite_height,
            main_dude_spritesheet_type,
            main_dude_physical_width,
            main_dude_physical_height,
            CreatureType::MAIN_DUDE
    ) {
        _friction = ICollidable::default_friction * 5.0f;
        _bouncing_factor_y = 0;
        _bouncing_factor_x = 0;
        init_sprites();
        whip = new Whip(0, 0);
        _gravity = ICollidable::default_gravity * 1.15f;
    }

    ~MainDude() {
        delete whip;
    }

    // Base creature overrides
    void update_creature_specific() override;

    void introduce_yourself() override { printf("MAIN_DUDE\n"); };

    void apply_dmg(int dmg_to_apply) override;

    bool can_update_collidable() const override { return true; }

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_apply_friction() const override { return true; }

    bool can_apply_gravity() const override;

    // Other, creature specific

    // Updates sprites' graphics according to current animation frame index.
    void match_animation();

    // Sets dead as true and effecively launches game summary.
    void set_dead();

    void boost_going_through_map_holes(MapTile **const t);

    void handle_key_input();

    void can_hang_on_tile(MapTile **neighboringTiles);

    void set_sprite_crawling();

    void set_sprite_hanging_on_tile();

    void set_sprite_whiping();

    void set_sprite_pushing();

    void set_sprite_stunned();

    void set_sprite_climbing();

    void set_sprite_dead();

    void set_sprite_walking_when_in_air();

    void set_sprite_falling();

    void set_sprite_exiting_level();

    void apply_blinking_on_damage();

    void reset_values_checked_every_frame();

    void throw_item();

    void take_out_bomb();

    void throw_rope();

    void spawn_carried_items();

    BaseItem *_currently_held_item{};
    BaseCreature *_currently_held_creature{};
    IPickupable *_currently_held_pickupable{};


    SpriteInfo *main_sprite_info{};
    SpriteInfo *sub_sprite_info{};

    int current_x_in_tiles{};
    int current_y_in_tiles{};

    double animation_frame_timer{};
    double speed_inc_timer{};
    double pos_inc_timer{};
    //How much time_utils was spent on hanging, since last flag hanging_on_tile_left/right occured.
    double hanging_timer{};

    double stunned_timer{};
    double jumping_timer{};
    double pushing_timer{};
    double time_since_last_jump{};
    double time_since_last_damage = MAIN_DUDE_DAMAGE_PROTECTION_TIME + 1;

    bool hanging_on_tile_left{};
    bool hanging_on_tile_right{};
    bool stunned{};
    bool crawling{};
    bool pushing_left{};
    bool pushing_right{};

    bool using_whip{};

    bool can_climb_rope{};
    bool can_climb_ladder{};
    bool climbing{};
    bool on_top_of_climbing_space{};
    bool exiting_level{};
    bool holding_item{};
    bool dead{};

    int animFrame{};
    u8 *frameGfx{};
    Whip *whip{};

    double climbing_timer{};
    int climbing_sound{};

    bool started_climbing_rope{};
    bool started_climbing_ladder{};

    bool carrying_spring_shoes{};
    bool carrying_spike_shoes{};
    bool carrying_mitt{};
    bool carrying_glove{};
    bool carrying_compass{};
    bool carrying_pistol{};
    bool carrying_shotgun{};

    bool carrying_cape{};
    bool using_cape{};
    bool carrying_jetpack{};
    bool using_jetpack{};
    int jetpack_fuel_counter{};

    bool carrying_damsel{};
};


#endif //SPELUNKYDS_MAINDUDE_H
