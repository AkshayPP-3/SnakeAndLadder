#include "raylib.h"
#include "board.hpp"
#include "game.hpp"
#include "dice.hpp"
#include <cstdlib>
#include <ctime>
int main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;
    
    srand(time(0));

    Game game;
    Dice dice;

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