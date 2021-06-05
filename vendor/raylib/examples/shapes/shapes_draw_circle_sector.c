/*******************************************************************************************
*
*   raylib [shapes] example - draw circle sector (with gui options)
*
*   This example has been created using raylib 2.5 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Example contributed by Vlad Adrian (@demizdor) and reviewed by Ramon Santamaria (@raysan5)
*
*   Copyright (c) 2018 Vlad Adrian (@demizdor) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"                 // Required for GUI controls

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [shapes] example - draw circle sector");

    Vector2 center = {(GetScreenWidth() - 300)/2, GetScreenHeight()/2 };

    float outerRadius = 180.0f;
    float startAngle = 0.0f;
    float endAngle = 180.0f;
    int segments = 0;
    int minSegments = 4;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // NOTE: All variables update happens inside GUI control functions
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawLine(500, 0, 500, GetScreenHeight(), Fade(LIGHTGRAY, 0.6f));
            DrawRectangle(500, 0, GetScreenWidth() - 500, GetScreenHeight(), Fade(LIGHTGRAY, 0.3f));

            DrawCircleSector(center, outerRadius, startAngle, endAngle, segments, Fade(MAROON, 0.3));
            DrawCircleSectorLines(center, outerRadius, startAngle, endAngle, segments, Fade(MAROON, 0.6));

            // Draw GUI controls
            //------------------------------------------------------------------------------
            startAngle = GuiSliderBar((Rectangle){ 600, 40, 120, 20}, "StartAngle", NULL, startAngle, 0, 720);
            endAngle = GuiSliderBar((Rectangle){ 600, 70, 120, 20}, "EndAngle", NULL, endAngle, 0, 720);

            outerRadius = GuiSliderBar((Rectangle){ 600, 140, 120, 20}, "Radius", NULL, outerRadius, 0, 200);
            segments = GuiSliderBar((Rectangle){ 600, 170, 120, 20}, "Segments", NULL, segments, 0, 100);
            //------------------------------------------------------------------------------

            minSegments = (int)ceilf((endAngle - startAngle) / 90);
            DrawText(TextFormat("MODE: %s", (segments >= minSegments)? "MANUAL" : "AUTO"), 600, 200, 10, (segments >= minSegments)? MAROON : DARKGRAY);

            DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}