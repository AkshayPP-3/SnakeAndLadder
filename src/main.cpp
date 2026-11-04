#include "raylib.h"
#include "board.hpp"
#include "game.hpp"
#include <cstdlib>
#include <ctime>

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    srand(static_cast<unsigned int>(time(0)));

    InitWindow(screenWidth, screenHeight, "Snake and Ladder");

    SetTargetFPS(60);

    Game game;

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