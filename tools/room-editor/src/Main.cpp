#include <cstdlib>
#include <string>
#include <iostream>

#include "raylib.h"
#include "MapTileType.hpp"
#include "MapTileToString.hpp"

namespace
{
    Texture2D tiles[static_cast<std::size_t>(MapTileType::_SIZE)];
    std::size_t selected_tile_index = -1;
    Camera2D camera = { 0 };
    Vector2 mouse_point = { 0.0f, 0.0f };
    const float tile_width = 16;
    const float tile_height = 16;
    const int screenWidth = 1280;
    const int screenHeight = 720;

    void load_tiles(const std::string& assetsPath)
    {
        for (auto index = static_cast<std::size_t>(MapTileType::NOTHING); index < static_cast<std::size_t>(MapTileType::_SIZE); index++)
        {
            const std::string path = assetsPath + "/assets/tilesheets/level-tiles/" + std::to_string(index) + ".png";
            std::cout << "Loading tile from: " << path << std::endl;

            auto& tile = tiles[index];

            tile = LoadTexture(path.c_str());
            std::cout << "Params: " << tile.width << ", " << tile.height << ", " << tile.id << std::endl;
        }
    }

    void display_menu()
    {
        Vector2 position {0, 0}; // By convention, it's a left-upper corner of the rectangle

        std::size_t index = 0;

        for (const auto& tile : tiles)
        {
            Rectangle dimensions{0, 0, tile_width, tile_height};

            // Check button state
            if (CheckCollisionPointRec(mouse_point, {camera.zoom * position.x, camera.zoom * position.y,
                                                     camera.zoom * dimensions.width, camera.zoom * dimensions.height}))
            {
                selected_tile_index = index;

                if (IsMouseButtonDown(MouseButton::MOUSE_LEFT_BUTTON))
                {
                    DrawTextureRec(tile, {0, 0, dimensions.width * 1.1f, dimensions.height * 1.1f}, position, BLUE);
                }
                else
                {
                    DrawTextureRec(tile, {0, 0, dimensions.width * 1.1f, dimensions.height * 1.1f}, position, GREEN);
                }
            }
            else
            {
                DrawTextureRec(tile, dimensions, position, WHITE);
            }

            if (position.x == 0)
            {
                position.x += (tile_width + 2);
            }
            else
            {
                position.y += (tile_height + 2);
                position.x = 0;
            }

            index++;
        }
    }

    bool has_selected_tile() { return selected_tile_index != -1; }
    Texture2D& get_selected_tile_texture() { assert(has_selected_tile()); return tiles[selected_tile_index]; }
}

int main()
{
    InitWindow(screenWidth, screenHeight, "SpelunkyPSP room editor");

    load_tiles("/home/dbeef/Desktop/spelunky-psp/"); // FIXME: Maybe pass via cmdline args? With fallback via relative paths?

    camera.target = {};
    camera.offset = (Vector2){ 2, 2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.6f;

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        mouse_point = GetMousePosition();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(camera);

        display_menu();

        EndMode2D();
        EndDrawing();
    }

    CloseWindow();
    return EXIT_SUCCESS;
}
