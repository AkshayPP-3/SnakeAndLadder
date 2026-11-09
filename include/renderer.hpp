
#pragma once

#include "raylib.h"

struct Renderer
{
    Texture2D player1;
    Texture2D player2;

    Texture2D background;
    Texture2D diceSheet;
    Texture2D labelP1;
    Texture2D labelP2;
    Texture2D settingsIcon;

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
