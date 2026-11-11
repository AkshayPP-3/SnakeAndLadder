
#pragma once

#include "raylib.h"

#include <vector>

struct Renderer
{
    Texture2D player1;
    Texture2D player2;

    Texture2D background;
    Texture2D diceSheet;
    Texture2D labelP1;
    Texture2D labelP2;
    Texture2D settingsIcon;

    Font winFont;

    Texture2D snakeTex[9];
    Texture2D ladderTex[8];
};

void LoadRenderer(Renderer& renderer);
void UnloadRenderer(Renderer& renderer);

void DrawRenderer(
    const Renderer& renderer,
    int diceFace1,
    int diceFace2,
    Vector2 pawn1Position,
    Vector2 pawn2Position
);

Vector2 GetPawnHome(int playerNumber);
Vector2 GetPawnAnchor(int cell);  

Rectangle GetPlayer1DiceBounds();
Rectangle GetPlayer2DiceBounds();

struct Confetti
{
    Vector2 position;
    float speed;
    float swayAmount;
    float swayPhase;
    float swaySpeed;
    float rotation;
    float rotationSpeed;
    float width;
    float height;
    Color color;
};

struct WinAnimation
{
    bool active = false;
    int winner = 0;
    float timer = 0.0f;
    std::vector<Confetti> confetti;
};

void StartWinAnimation(WinAnimation& win, int winner);
void UpdateWinAnimation(WinAnimation& win, float deltaTime);
void DrawWinAnimation(const Renderer& renderer, const WinAnimation& win);

bool IsPlayAgainPressed(const WinAnimation& win);
