#include "Collisions.hpp"

#include <cstdint>
#include <cmath>

// XY axes go as following:
// ____ +x
// |
// |
// +y
//
// Rendering dimensions are same as physical dimensions, in other words,
// a quad made of 1x1 vertices will have a collision box of 1x1.

void collisions::get_neighbouring_tiles(Level &level, float x, float y, MapTile *out_neighboring_tiles[9])
{
    const float width = 1.0f;
    const float height = 1.0f;

    std::uint16_t x_tiles = std::floor(x + (width / 2));
    std::uint16_t y_tiles = std::floor(y + (height / 2));

    if (x_tiles >= 32 || x_tiles < 0 || y_tiles >= 32 || y_tiles < 0)
    {
        return;
    }

    const auto& tiles = level.map_tiles;

    MapTile *left_middle = nullptr,
            *right_middle = nullptr,
            *up_middle = nullptr,
            *down_middle = nullptr,
            *center = nullptr,
            *left_up = nullptr,
            *right_up = nullptr,
            *left_down = nullptr,
            *right_down = nullptr;

    left_middle = x_tiles - 1 >= 0 && tiles[x_tiles - 1][y_tiles]->exists ? tiles[x_tiles - 1][y_tiles] : nullptr;
    right_middle = x_tiles + 1 <= 31 && tiles[x_tiles + 1][y_tiles]->exists ? tiles[x_tiles + 1][y_tiles] : nullptr;
    up_middle = y_tiles - 1 >= 0 && tiles[x_tiles][y_tiles - 1]->exists ? tiles[x_tiles][y_tiles - 1] : nullptr;
    down_middle = y_tiles + 1 <= 31 && tiles[x_tiles][y_tiles + 1]->exists ? tiles[x_tiles][y_tiles + 1] : nullptr;
    center = tiles[x_tiles][y_tiles]->exists ? tiles[x_tiles][y_tiles] : nullptr;
    left_up = x_tiles - 1 >= 0 && y_tiles - 1 >= 0 && tiles[x_tiles - 1][y_tiles - 1]->exists ? tiles[x_tiles - 1][y_tiles - 1] : nullptr;
    right_up = x_tiles + 1 <= 31 && y_tiles - 1 >= 0 && tiles[x_tiles + 1][y_tiles - 1]->exists ? tiles[x_tiles + 1][y_tiles - 1] : nullptr;
    left_down = x_tiles - 1 >= 0 && y_tiles + 1 <= 31 && tiles[x_tiles - 1][y_tiles + 1]->exists ? tiles[x_tiles - 1][y_tiles + 1] : nullptr;
    right_down = x_tiles + 1 <= 31 && y_tiles + 1 <= 31 && tiles[x_tiles + 1][y_tiles + 1]->exists ? tiles[x_tiles + 1][y_tiles + 1] : nullptr;

    out_neighboring_tiles[static_cast<uint16_t>(NeighbouringTiles::LEFT_MIDDLE)] = left_middle;
    out_neighboring_tiles[static_cast<uint16_t>(NeighbouringTiles::RIGHT_MIDDLE)] = right_middle;
    out_neighboring_tiles[static_cast<uint16_t>(NeighbouringTiles::UP_MIDDLE)] = up_middle;
    out_neighboring_tiles[static_cast<uint16_t>(NeighbouringTiles::DOWN_MIDDLE)] = down_middle;
    out_neighboring_tiles[static_cast<uint16_t>(NeighbouringTiles::CENTER)] = center;
    out_neighboring_tiles[static_cast<uint16_t>(NeighbouringTiles::LEFT_UP)] = left_up;
    out_neighboring_tiles[static_cast<uint16_t>(NeighbouringTiles::RIGHT_UP)] = right_up;
    out_neighboring_tiles[static_cast<uint16_t>(NeighbouringTiles::LEFT_DOWN)] = left_down;
    out_neighboring_tiles[static_cast<uint16_t>(NeighbouringTiles::RIGHT_DOWN)] = right_down;
}

MapTile *collisions::check_bottom_collision(MapTile **neighboring_tiles, float x_center, float y_center, float width, float height)
{
    bool condition_x;
    bool condition_y;

    const float half_w = width / 2;
    const float half_h = height / 2;

    const float tile_w = 1.0f;
    const float tile_h = 1.0f;

    for (int a = 0; a < 9; a++) {

        if (neighboring_tiles[a] == nullptr)
        {
            continue;
        }

        if (!neighboring_tiles[a]->collidable)
        {
            continue;
        }

        condition_x = x_center + half_w >= neighboring_tiles[a]->x && x_center < neighboring_tiles[a]->x + tile_w + half_w;
        condition_y = y_center + half_h >= neighboring_tiles[a]->y && y_center + half_h < neighboring_tiles[a]->y + tile_h;

        if (condition_x && condition_y)
        {
            return neighboring_tiles[a];
        }
    }

    return nullptr;
}

MapTile *collisions::check_top_collision(MapTile **neighboring_tiles, float x_center, float y_center, float width, float height)
{
    bool condition_x;
    bool condition_y;

    const float half_w = width / 2;
    const float half_h = height / 2;

    const float tile_w = 1.0f;
    const float tile_h = 1.0f;

    for (int a = 0; a < 9; a++) {

        if (neighboring_tiles[a] == nullptr)
        {
            continue;
        }

        if (!neighboring_tiles[a]->collidable)
        {
            continue;
        }

        condition_x = x_center + half_w >= neighboring_tiles[a]->x && x_center < neighboring_tiles[a]->x + tile_w + half_w;
        condition_y = neighboring_tiles[a]->y + tile_h >= y_center - half_h && y_center - half_h > neighboring_tiles[a]->y;

        if (condition_x && condition_y)
        {
            return neighboring_tiles[a];
        }
    }

    return nullptr;
}

MapTile *collisions::check_right_collision(MapTile **neighboring_tiles, float x_center, float y_center, float width, float height)
{
    bool condition_x;
    bool condition_y;

    const float half_w = width / 2;
    const float half_h = height / 2;

    const float tile_w = 1.0f;
    const float tile_h = 1.0f;

    for (int a = 0; a < 9; a++) {

        if (neighboring_tiles[a] == nullptr)
        {
            continue;
        }

        if (!neighboring_tiles[a]->collidable)
        {
            continue;
        }

        condition_x = x_center + half_w >= neighboring_tiles[a]->x && x_center + half_w < neighboring_tiles[a]->x + tile_w;
        condition_y = y_center + half_h >= neighboring_tiles[a]->y && y_center - half_h <= neighboring_tiles[a]->y + tile_h;

        if (condition_x && condition_y)
        {
            return neighboring_tiles[a];
        }
    }

    return nullptr;
}

MapTile *collisions::check_left_collision(MapTile **neighboring_tiles, float x_center, float y_center, float width, float height)
{
    bool condition_x;
    bool condition_y;

    const float half_w = width / 2;
    const float half_h = height / 2;

    const float tile_w = 1.0f;
    const float tile_h = 1.0f;

    for (int a = 0; a < 9; a++) {

        if (neighboring_tiles[a] == nullptr)
        {
            continue;
        }

        if (!neighboring_tiles[a]->collidable)
        {
            continue;
        }

        condition_x = x_center - half_w <= neighboring_tiles[a]->x + tile_w && x_center - half_w > neighboring_tiles[a]->x;
        condition_y = y_center + half_h >= neighboring_tiles[a]->y && y_center - half_h <= neighboring_tiles[a]->y + tile_h;

        if (condition_x && condition_y)
        {
            return neighboring_tiles[a];
        }
    }

    return nullptr;
}
