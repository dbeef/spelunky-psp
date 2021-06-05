/*******************************************************************************************
*
*   raylib [models] example - Waving cubes
*
*   This example has been created using raylib 2.5 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Example contributed by Codecat (@codecat) and reviewed by Ramon Santamaria (@raysan5)
*
*   Copyright (c) 2019 Codecat (@codecat) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include <math.h>

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [models] example - waving cubes");

    // Initialize the camera
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 30.0f, 20.0f, 30.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 70.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Specify the amount of blocks in each direction
    const int numBlocks = 15;

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        double time = GetTime();

        // Calculate time scale for cube position and size
        float scale = (2.0f + (float)sin(time))*0.7f;

        // Move camera around the scene
        double cameraTime = time*0.3;
        camera.position.x = (float)cos(cameraTime)*40.0f;
        camera.position.z = (float)sin(cameraTime)*40.0f;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawGrid(10, 5.0f);

                for (int x = 0; x < numBlocks; x++)
                {
                    for (int y = 0; y < numBlocks; y++)
                    {
                        for (int z = 0; z < numBlocks; z++)
                        {
                            // Scale of the blocks depends on x/y/z positions
                            float blockScale = (x + y + z)/30.0f;

                            // Scatter makes the waving effect by adding blockScale over time
                            float scatter = sinf(blockScale*20.0f + (float)(time*4.0f));

                            // Calculate the cube position
                            Vector3 cubePos = {
                                (float)(x - numBlocks/2)*(scale*3.0f) + scatter,
                                (float)(y - numBlocks/2)*(scale*2.0f) + scatter,
                                (float)(z - numBlocks/2)*(scale*3.0f) + scatter
                            };

                            // Pick a color with a hue depending on cube position for the rainbow color effect
                            Color cubeColor = ColorFromHSV((float)(((x + y + z)*18)%360), 0.75f, 0.9f);

                            // Calculate cube size
                            float cubeSize = (2.4f - scale)*blockScale;

                            // And finally, draw the cube!
                            DrawCube(cubePos, cubeSize, cubeSize, cubeSize, cubeColor);
                        }
                    }
                }

            EndMode3D();

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
