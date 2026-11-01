#include "raylib.h"
#include "board.hpp"
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
        DrawBoard();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}