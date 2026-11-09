
#include "raylib.h"
#include "game.hpp"
#include "renderer.hpp"

#include <cstdlib>
#include <ctime>

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    srand(static_cast<unsigned int>(time(0)));

    InitWindow(screenWidth, screenHeight, "Snake and Ladder");
    SetTargetFPS(60);

    Renderer renderer{};
    LoadRenderer(renderer);

    Game game;

    int diceFace1 = 1;
    int diceFace2 = 1;

    bool isRolling = false;
    int rollingPlayer = 0;

    float rollTimer = 0.0f;
    float faceTimer = 0.0f;

    const float rollDuration = 1.0f;
    const float faceInterval = 0.08f;

    int finalDiceValue = 1;

    while (!WindowShouldClose())
    {
        if (!isRolling && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mousePosition = GetMousePosition();

            if (CheckCollisionPointRec(mousePosition, GetPlayer1DiceBounds())
                && game.getCurrentPlayerNumber() == 1)
            {
                isRolling = true;
                rollingPlayer = 1;
            }
            else if (CheckCollisionPointRec(mousePosition, GetPlayer2DiceBounds())
                     && game.getCurrentPlayerNumber() == 2)
            {
                isRolling = true;
                rollingPlayer = 2;
            }
        }

        if (isRolling)
        {
            float deltaTime = GetFrameTime();

            rollTimer += deltaTime;
            faceTimer += deltaTime;

            if (faceTimer >= faceInterval)
            {
                int randomFace = rand() % 6 + 1;

                if (rollingPlayer == 1)
                    diceFace1 = randomFace;
                else
                    diceFace2 = randomFace;

                faceTimer = 0.0f;
            }

            if (rollTimer >= rollDuration)
            {
                finalDiceValue = rand() % 6 + 1;

                if (rollingPlayer == 1)
                    diceFace1 = finalDiceValue;
                else
                    diceFace2 = finalDiceValue;

                isRolling = false;

                rollTimer = 0.0f;
                faceTimer = 0.0f;

                game.switchTurn();

                TraceLog(LOG_INFO, "Dice result: %d", finalDiceValue);
                TraceLog(LOG_INFO, "Next player: %d",
                         game.getCurrentPlayerNumber());
            }
        }

        BeginDrawing();

        DrawRenderer(renderer, diceFace1, diceFace2);

        EndDrawing();
    }

    UnloadRenderer(renderer);

    CloseWindow();

    return 0;
}
