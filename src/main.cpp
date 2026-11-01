#include "raylib.h"

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Snake and Ladder");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawRectangle(350, 250, 60, 80, RED);
        DrawRectangleLines(350, 250, 60, 80, BLACK);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}