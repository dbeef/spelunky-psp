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
    camera.zoom = 1.65f;

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(camera);

        const float tile_width = 16;
        const float tile_height = 16;
        Vector2 position {0, 0}; // By convention, it's a left-upper corner of the rectangle

        for (const auto& tile : tiles)
        {
            Rectangle dimensions{0, 0, tile_width, tile_height};
            DrawTextureRec(tile, dimensions, position, WHITE);

            if (position.x == 0)
            {
                position.x += (tile_width + 2);
            }
            else
            {
                position.y += (tile_height + 2);
                position.x = 0;
            }
        }

        EndMode2D();
        EndDrawing();
    }

    CloseWindow();
    return EXIT_SUCCESS;
}

/*
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "raylib [textures] example - sprite button");
    InitAudioDevice();      // Initialize audio device

    Texture2D button = LoadTexture("/home/dbeef/Desktop/spelunky-psp/assets/tilesheets/level-tiles/2.png"); // Load button texture

    // Define frame rectangle for drawing
    float frameHeight = (float)button.height/NUM_FRAMES;
    Rectangle sourceRec = { 0, 0, (float)button.width, frameHeight };

    // Define button bounds on screen
    Rectangle btnBounds = { screenWidth/2.0f - button.width/2.0f, screenHeight/2.0f - button.height/NUM_FRAMES/2.0f, (float)button.width, frameHeight };

    int btnState = 0;               // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool btnAction = false;         // Button action should be activated

    Vector2 mousePoint = { 0.0f, 0.0f };

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        mousePoint = GetMousePosition();
        btnAction = false;

        // Check button state
        if (CheckCollisionPointRec(mousePoint, btnBounds))
        {
            if (IsMouseButtonDown(MouseButton::MOUSE_LEFT_BUTTON)) btnState = 2;
            else btnState = 1;

            if (IsMouseButtonReleased(MouseButton::MOUSE_LEFT_BUTTON)) btnAction = true;
        }
        else btnState = 0;

        if (btnAction)
        {
            // TODO: Any desired action
        }

        // Calculate button frame rectangle to draw depending on button state
        sourceRec.y = btnState*frameHeight;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTextureRec(button, sourceRec, (Vector2){ btnBounds.x, btnBounds.y }, WHITE); // Draw button frame
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(button);  // Unload button texture

    CloseAudioDevice();     // Close audio device

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
*/
