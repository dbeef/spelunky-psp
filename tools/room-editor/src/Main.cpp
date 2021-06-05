/*******************************************************************************************
*
*   raylib [textures] example - sprite button
*
*   This example has been created using raylib 2.5 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2019 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <cstdlib>
#include <string>
#include <iostream>

#include "raylib.h"
#include "MapTileType.hpp"
#include "MapTileToString.hpp"

namespace
{
    Texture2D tiles[static_cast<std::size_t>(MapTileType::_SIZE)];
}

int main()
{
    const std::string assetsPath = "/home/dbeef/Desktop/spelunky-psp/";

    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "SpelunkyPSP room editor");

    // Load the tiles:

    for (auto index = static_cast<std::size_t>(MapTileType::NOTHING); index < static_cast<std::size_t>(MapTileType::_SIZE); index++)
    {
        const std::string path = assetsPath + "/assets/tilesheets/level-tiles/" + std::to_string(index) + ".png";
        std::cout << "Loading tile from: " << path << std::endl;

        auto& tile = tiles[index];

        tile = LoadTexture(path.c_str());
        std::cout << "Params: " << tile.width << ", " << tile.height << ", " << tile.id << std::endl;
    }

    // Render loop:

    Camera2D camera = { 0 };
    camera.target = {};
    camera.offset = (Vector2){ 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.6f;

    Vector2 mouse_point = { 0.0f, 0.0f };

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        mouse_point = GetMousePosition();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(camera);

        const float tile_width = 16;
        const float tile_height = 16;
        Vector2 position {0, 0}; // By convention, it's a left-upper corner of the rectangle

        std::size_t index = 0;

        for (const auto& tile : tiles)
        {
            Rectangle dimensions{0, 0, tile_width, tile_height};
            DrawTextureRec(tile, dimensions, position, WHITE);

            // Check button state
            if (CheckCollisionPointRec(mouse_point, {camera.zoom * position.x, camera.zoom * position.y,
                                                     camera.zoom * dimensions.width, camera.zoom * dimensions.height}))
            {
                std::cout << "Hovering above tile: " << toString(static_cast<MapTileType>(index)) << std::endl;

                if (IsMouseButtonDown(MouseButton::MOUSE_LEFT_BUTTON)) {}
                if (IsMouseButtonReleased(MouseButton::MOUSE_LEFT_BUTTON)) {}
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

        EndMode2D();
        EndDrawing();
    }

    CloseWindow();
    return EXIT_SUCCESS;
}
