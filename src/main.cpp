
#include "raylib.h"
#include "game.hpp"
#include "renderer.hpp"
#include "movement.hpp"

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

    // Pawn animation: starts outside the board, beside each dice
    Movement pawn1;
    Movement pawn2;
    pawn1.setPosition(GetPawnHome(1));
    pawn2.setPosition(GetPawnHome(2));

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
        // Wait until the pawns stop moving before the next roll
        bool pawnsMoving = pawn1.isMoving() || pawn2.isMoving();

        if (!isRolling && !pawnsMoving && !game.isGameOver()
            && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
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

                // Apply the dice value to the game, then animate the pawn
                TurnResult result = game.playTurn(finalDiceValue);

                if (result.player == 1)
                    PlanPawnMove(pawn1, result);
                else
                    PlanPawnMove(pawn2, result);

                TraceLog(LOG_INFO, "Dice result: %d", finalDiceValue);
                TraceLog(LOG_INFO, "Next player: %d",
                         game.getCurrentPlayerNumber());
            }
        }

        pawn1.update(GetFrameTime());
        pawn2.update(GetFrameTime());

        BeginDrawing();

        DrawRenderer(renderer, diceFace1, diceFace2,
                     pawn1.getPosition(), pawn2.getPosition());

        EndDrawing();
    }

    UnloadRenderer(renderer);

    CloseWindow();

    return 0;
}
