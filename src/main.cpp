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

    // Load images
    Texture2D player1 = LoadTexture("assets/images/player1.png");
    Texture2D player2 = LoadTexture("assets/images/player2.png");
    Texture2D snake = LoadTexture("assets/images/snake.png");
    Texture2D ladder = LoadTexture("assets/images/ladder.png");

    Game game;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawBoard();

        // Display images (temporary test positions)
        DrawTextureEx(player1, {100, 100}, 0.0f, 0.15f, WHITE);
        DrawTextureEx(player2, {100, 200}, 0.0f, 0.15f, WHITE);
        DrawTextureEx(snake, {300, 100}, 0.0f, 0.15f, WHITE);
        DrawTextureEx(ladder, {500, 100}, 0.0f, 0.15f, WHITE);

        EndDrawing();
    }

    // Release image memory
    UnloadTexture(player1);
    UnloadTexture(player2);
    UnloadTexture(snake);
    UnloadTexture(ladder);

    CloseWindow();

    return 0;
}