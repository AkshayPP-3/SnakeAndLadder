#include "raylib.h"
#include "board.hpp"
#include "game.hpp"

#include <cstdlib>
#include <ctime>
#include <cmath>

// Board layout (must match DrawBoard)
const int BOARD_X = 150;
const int BOARD_Y = 50;
const int CELL    = 50;

// Cell number -> center of that cell on screen
Vector2 CellCenter(int n)
{
    int idx = n - 1;
    int row = idx / 10;                 // 0 = bottom row
    int col = idx % 10;
    if (row % 2 == 1) col = 9 - col;    // zig-zag numbering

    return {
        (float)(BOARD_X + col * CELL + CELL / 2),
        (float)(BOARD_Y + (9 - row) * CELL + CELL / 2)
    };
}

// A sprite (snake or ladder) stretched between two cells.
//   from/to      : cell numbers
//   file         : image of this one sprite
//   fromPoint    : point inside the image that lands on the "from" cell center
//   toPoint      : point inside the image that lands on the "to" cell center
struct SpriteDef
{
    int from;
    int to;
    const char *file;
    Vector2 fromPoint;
    Vector2 toPoint;
};

// snakes[0] .. snakes[8]  ->  {head, tail}
const SpriteDef SNAKE_DEFS[9] = {
    {27,  5, "assets/images/snakes/snake_0.png", {126.8f, 11.9f}, {1.1f, 76.8f}},
    {38, 17, "assets/images/snakes/snake_1.png", {28.9f, 14.5f}, {31.2f, 126.3f}},
    {52, 33, "assets/images/snakes/snake_2.png", {70.8f, 15.5f}, {7.5f, 107.6f}},
    {66, 46, "assets/images/snakes/snake_3.png", {29.5f, 14.3f}, {26.3f, 114.2f}},
    {90, 70, "assets/images/snakes/snake_4.png", {26.8f, 12.0f}, {37.5f, 111.5f}},
    {93, 67, "assets/images/snakes/snake_5.png", {42.2f, 13.2f}, {4.0f, 166.7f}},
    {95, 75, "assets/images/snakes/snake_6.png", {33.8f, 16.1f}, {40.1f, 115.9f}},
    {97, 58, "assets/images/snakes/snake_7.png", {57.9f, 13.9f}, {1.6f, 212.2f}},
    {99, 81, "assets/images/snakes/snake_8.png", {53.1f, 11.0f}, {1.0f, 58.7f}},
};

// ladders[0] .. ladders[7]  ->  {bottom, top}
const SpriteDef LADDER_DEFS[8] = {
    { 2, 18, "assets/images/ladders/ladder_0.png", {18.0f, 80.0f}, {18.0f, 9.0f}},
    { 9, 28, "assets/images/ladders/ladder_1.png", {18.0f, 121.0f}, {18.0f, 9.0f}},
    {25, 47, "assets/images/ladders/ladder_2.png", {18.0f, 150.0f}, {18.0f, 9.0f}},
    {30, 50, "assets/images/ladders/ladder_3.png", {18.0f, 109.0f}, {18.0f, 9.0f}},
    {45, 57, "assets/images/ladders/ladder_4.png", {18.0f, 80.0f}, {18.0f, 9.0f}},
    {51, 73, "assets/images/ladders/ladder_5.png", {18.0f, 150.0f}, {18.0f, 9.0f}},
    {65, 85, "assets/images/ladders/ladder_6.png", {18.0f, 109.0f}, {18.0f, 9.0f}},
    {79, 98, "assets/images/ladders/ladder_7.png", {18.0f, 121.0f}, {18.0f, 9.0f}},
};

// Draw a sprite so that fromPoint sits on cell "from" and toPoint on cell "to"
void DrawBetweenCells(const SpriteDef &s, Texture2D tex)
{
    Vector2 a = CellCenter(s.from);
    Vector2 b = CellCenter(s.to);

    float bx = b.x - a.x, by = b.y - a.y;                       // board vector
    float sx = s.toPoint.x - s.fromPoint.x;                     // image vector
    float sy = s.toPoint.y - s.fromPoint.y;

    float scale    = std::sqrt(bx * bx + by * by) / std::sqrt(sx * sx + sy * sy);
    float rotation = (std::atan2(by, bx) - std::atan2(sy, sx)) * RAD2DEG;

    Rectangle src  = {0, 0, (float)tex.width, (float)tex.height};
    Rectangle dest = {a.x, a.y, tex.width * scale, tex.height * scale};
    Vector2 origin = {s.fromPoint.x * scale, s.fromPoint.y * scale};  // rotate around the start point

    DrawTexturePro(tex, src, dest, origin, rotation, WHITE);
}

// ---------- UI layout (800 x 600 window) ----------
const float DICE_SIZE = 80.0f;
const float DICE_Y    = 290.0f;      // vertical center of both dice
const float DICE1_X   = 79.0f;       // left of the board  (Player 1)
const float DICE2_X   = 721.0f;      // right of the board (Player 2)

// Draw one face (1..6) of the dice sheet, centered at (cx, cy)
void DrawDice(Texture2D sheet, int face, float cx, float cy)
{
    float fw = sheet.width / 6.0f;                       // 6 faces in one row
    Rectangle src  = {(face - 1) * fw, 0, fw, (float)sheet.height};
    Rectangle dest = {cx - DICE_SIZE / 2, cy - DICE_SIZE / 2, DICE_SIZE, DICE_SIZE};
    DrawTexturePro(sheet, src, dest, {0, 0}, 0.0f, WHITE);
}

// Draw a label image centered at cx, with its top edge at y
void DrawLabel(Texture2D label, float cx, float y)
{
    float w = label.width * 0.5f;                            // images are 2x size
    DrawTextureEx(label, {cx - w / 2, y}, 0.0f, 0.5f, WHITE);
}

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

    // UI images
    Texture2D background = LoadTexture("assets/images/ui/background.png");
    Texture2D diceSheet  = LoadTexture("assets/images/ui/dice_faces.png");
    Texture2D labelP1    = LoadTexture("assets/images/ui/label_p1.png");
    Texture2D labelP2    = LoadTexture("assets/images/ui/label_p2.png");
    Texture2D settingsIcon = LoadTexture("assets/images/ui/settings.png");
    SetTextureFilter(diceSheet,  TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(labelP1,    TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(labelP2,    TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(settingsIcon, TEXTURE_FILTER_BILINEAR);

    int diceFace1 = 1;   // face currently shown on Player 1's dice
    int diceFace2 = 1;   // face currently shown on Player 2's dice

    Texture2D snakeTex[9];
    for (int i = 0; i < 9; i++)
    {
        snakeTex[i] = LoadTexture(SNAKE_DEFS[i].file);
        SetTextureFilter(snakeTex[i], TEXTURE_FILTER_BILINEAR);
    }

    Texture2D ladderTex[8];
    for (int i = 0; i < 8; i++)
    {
        ladderTex[i] = LoadTexture(LADDER_DEFS[i].file);
        SetTextureFilter(ladderTex[i], TEXTURE_FILTER_BILINEAR);
    }

    Game game;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Background (drawn first, behind everything)
        DrawTexture(background, 0, 0, WHITE);

        DrawBoard();

        // Ladders first, so snakes are drawn on top
        for (int i = 0; i < 8; i++)
            DrawBetweenCells(LADDER_DEFS[i], ladderTex[i]);

        // Snakes
        for (int i = 0; i < 9; i++)
            DrawBetweenCells(SNAKE_DEFS[i], snakeTex[i]);

        // Dice beside the board: Player 1 on the left, Player 2 on the right
        DrawDice(diceSheet, diceFace1, DICE1_X, DICE_Y);
        DrawLabel(labelP1, DICE1_X, DICE_Y + DICE_SIZE / 2 + 8);

        DrawDice(diceSheet, diceFace2, DICE2_X, DICE_Y);
        DrawLabel(labelP2, DICE2_X, DICE_Y + DICE_SIZE / 2 + 8);

        // Pawns: red under the Player 1 label, blue under the Player 2 label
        float pawnTop = DICE_Y + DICE_SIZE / 2 + 8 + 30 + 10;
        float p1Scale = (float)CELL / player1.height;
        float p2Scale = (float)CELL / player2.height;
        DrawTextureEx(player1, {DICE1_X - player1.width * p1Scale / 2, pawnTop}, 0.0f, p1Scale, WHITE);
        DrawTextureEx(player2, {DICE2_X - player2.width * p2Scale / 2, pawnTop}, 0.0f, p2Scale, WHITE);

        // Settings icon (top right)
        DrawTextureEx(settingsIcon, {724, 16}, 0.0f, 56.0f / settingsIcon.width, WHITE);

        EndDrawing();
    }

    // Release image memory
    UnloadTexture(player1);
    UnloadTexture(player2);
    UnloadTexture(background);
    UnloadTexture(diceSheet);
    UnloadTexture(labelP1);
    UnloadTexture(labelP2);
    UnloadTexture(settingsIcon);
    for (int i = 0; i < 9; i++) UnloadTexture(snakeTex[i]);
    for (int i = 0; i < 8; i++) UnloadTexture(ladderTex[i]);

    CloseWindow();

    return 0;
}
  