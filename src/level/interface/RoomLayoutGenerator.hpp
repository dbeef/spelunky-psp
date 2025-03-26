#pragma once

#include "RoomType.hpp"

namespace Consts
{
    const int ROOMS_COUNT_WIDTH = 3;
    const int ROOMS_COUNT_HEIGHT = 3;

    static_assert(ROOMS_COUNT_WIDTH > 1, "Level generator will fail with current horizontal rooms count.");
    static_assert(ROOMS_COUNT_HEIGHT > 1, "Level generator will fail with current vertical rooms count.");

    const int SPLASH_SCREEN_WIDTH_TILES = 20;
    const int SPLASH_SCREEN_HEIGHT_TILES = 12;

    const int ROOM_WIDTH_TILES = 10;
    const int ROOM_HEIGHT_TILES = 10;
}


class RoomLayoutGenerator {
public:
    struct RoomLayoutGeneratorParams {
        bool shopkeeper_robbed = false;
        int width_x_rooms = Consts::ROOMS_COUNT_WIDTH;
        int height_y_rooms = Consts::ROOMS_COUNT_HEIGHT;
        int room_width_tiles = Consts::ROOM_WIDTH_TILES;
        int room_height_tiles = Consts::ROOM_HEIGHT_TILES;
    };

    void generate(const RoomLayoutGeneratorParams &params);

    RoomType get_room_type_at_room(int x_room, int y_room) const;

    RoomType get_room_type_at_tile(int x_tile, int y_tile) const;

    int get_width_tiles() const;

    int get_height_tiles() const;

private:
    int _width_x_rooms = 0;
    int _height_y_rooms = 0;
    int _room_width_tiles = 0;
    int _room_height_tiles = 0;

    // Any encountered closed room will be turned into an altar.
    void place_an_altar();

    // Finds a closed room that is not blocked from either left or right side by other closed room,
    // and plants a shop there that is oriented to the not-blocked side.
    void place_a_shop(bool shopkeeper_robbed);

    RoomType _layout[3][3]{}; // fixme
    int _layout_room_ids[3][3]{}; // fixme
    // RoomType _layout[Consts::ROOMS_COUNT_WIDTH][Consts::ROOMS_COUNT_HEIGHT]{};
    // int _layout_room_ids[Consts::ROOMS_COUNT_WIDTH][Consts::ROOMS_COUNT_HEIGHT]{};
};
