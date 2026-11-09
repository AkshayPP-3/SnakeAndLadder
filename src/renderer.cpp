
#include "renderer.hpp"
#include "board.hpp"

#include <cmath>

const int BOARD_X = 150;
const int BOARD_Y = 50;
const int CELL = 50;

const float DICE_SIZE = 80.0f;
const float DICE_Y = 290.0f;
const float DICE1_X = 79.0f;
const float DICE2_X = 721.0f;

const float PAWN_HEIGHT       = 40.0f;   // pawn height on screen
const float PAWN_FOOT         = 18.0f;   // how far below the cell center the pawn's feet stand
const float PAWN_SPREAD       = 8.0f;    // sideways shift when both pawns share a cell
const float PAWN_SPREAD_RANGE = 30.0f;   // pawns closer than this start sliding apart

struct SpriteDef
{
    int from;
    int to;
    const char* file;
    Vector2 fromPoint;
    Vector2 toPoint;
};

const SpriteDef SNAKE_DEFS[9] = {
    {27, 5, "assets/images/snakes/snake_0.png", {126.8f, 11.9f}, {1.1f, 76.8f}},
    {38, 17, "assets/images/snakes/snake_1.png", {28.9f, 14.5f}, {31.2f, 126.3f}},
    {52, 33, "assets/images/snakes/snake_2.png", {70.8f, 15.5f}, {7.5f, 107.6f}},
    {66, 46, "assets/images/snakes/snake_3.png", {29.5f, 14.3f}, {26.3f, 114.2f}},
    {90, 70, "assets/images/snakes/snake_4.png", {26.8f, 12.0f}, {37.5f, 111.5f}},
    {93, 67, "assets/images/snakes/snake_5.png", {42.2f, 13.2f}, {4.0f, 166.7f}},
    {95, 75, "assets/images/snakes/snake_6.png", {33.8f, 16.1f}, {40.1f, 115.9f}},
    {97, 58, "assets/images/snakes/snake_7.png", {57.9f, 13.9f}, {1.6f, 212.2f}},
    {99, 81, "assets/images/snakes/snake_8.png", {53.1f, 11.0f}, {1.0f, 58.7f}}
};

const SpriteDef LADDER_DEFS[8] = {
    {2, 18, "assets/images/ladders/ladder_0.png", {18.0f, 80.0f}, {18.0f, 9.0f}},
    {9, 28, "assets/images/ladders/ladder_1.png", {18.0f, 121.0f}, {18.0f, 9.0f}},
    {25, 47, "assets/images/ladders/ladder_2.png", {18.0f, 150.0f}, {18.0f, 9.0f}},
    {30, 50, "assets/images/ladders/ladder_3.png", {18.0f, 109.0f}, {18.0f, 9.0f}},
    {45, 57, "assets/images/ladders/ladder_4.png", {18.0f, 80.0f}, {18.0f, 9.0f}},
    {51, 73, "assets/images/ladders/ladder_5.png", {18.0f, 150.0f}, {18.0f, 9.0f}},
    {65, 85, "assets/images/ladders/ladder_6.png", {18.0f, 109.0f}, {18.0f, 9.0f}},
    {79, 98, "assets/images/ladders/ladder_7.png", {18.0f, 121.0f}, {18.0f, 9.0f}}
};

Vector2 CellCenter(int n)
{
    int idx = n - 1;
    int row = idx / 10;
    int col = idx % 10;

    if (row % 2 == 1)
        col = 9 - col;

    return {
        (float)(BOARD_X + col * CELL + CELL / 2),
        (float)(BOARD_Y + (9 - row) * CELL + CELL / 2)
    };
}

Vector2 GetPawnHome(int playerNumber)
{
    float x = (playerNumber == 1) ? DICE1_X : DICE2_X;
    float y = DICE_Y + DICE_SIZE / 2 + 8 + 30 + 10 + PAWN_HEIGHT;

    return {x, y};
}

Vector2 GetPawnAnchor(int cell)
{
    Vector2 c = CellCenter(cell);

    return {c.x, c.y + PAWN_FOOT};
}

// Draw a pawn with its feet (bottom-center) at "anchor"
void DrawPawn(Texture2D tex, Vector2 anchor)
{
    float scale = PAWN_HEIGHT / tex.height;

    DrawTextureEx(
        tex,
        {anchor.x - tex.width * scale / 2, anchor.y - PAWN_HEIGHT},
        0.0f,
        scale,
        WHITE
    );
}

void DrawBetweenCells(const SpriteDef& s, Texture2D tex)
{
    Vector2 a = CellCenter(s.from);
    Vector2 b = CellCenter(s.to);

    float bx = b.x - a.x;
    float by = b.y - a.y;

    float sx = s.toPoint.x - s.fromPoint.x;
    float sy = s.toPoint.y - s.fromPoint.y;

    float scale = std::sqrt(bx * bx + by * by) /
                  std::sqrt(sx * sx + sy * sy);

    float rotation = (std::atan2(by, bx) -
                      std::atan2(sy, sx)) * RAD2DEG;

    Rectangle src = {
        0, 0,
        (float)tex.width,
        (float)tex.height
    };

    Rectangle dest = {
        a.x, a.y,
        tex.width * scale,
        tex.height * scale
    };

    Vector2 origin = {
        s.fromPoint.x * scale,
        s.fromPoint.y * scale
    };

    DrawTexturePro(tex, src, dest, origin, rotation, WHITE);
}

void DrawDice(Texture2D sheet, int face, float cx, float cy)
{
    float fw = sheet.width / 6.0f;

    Rectangle src = {
        (face - 1) * fw,
        0,
        fw,
        (float)sheet.height
    };

    Rectangle dest = {
        cx - DICE_SIZE / 2,
        cy - DICE_SIZE / 2,
        DICE_SIZE,
        DICE_SIZE
    };

    DrawTexturePro(sheet, src, dest, {0, 0}, 0.0f, WHITE);
}

void DrawLabel(Texture2D label, float cx, float y)
{
    float w = label.width * 0.5f;

    DrawTextureEx(
        label,
        {cx - w / 2, y},
        0.0f,
        0.5f,
        WHITE
    );
}

Rectangle GetPlayer1DiceBounds()
{
    return {
        DICE1_X - DICE_SIZE / 2,
        DICE_Y - DICE_SIZE / 2,
        DICE_SIZE,
        DICE_SIZE
    };
}

Rectangle GetPlayer2DiceBounds()
{
    return {
        DICE2_X - DICE_SIZE / 2,
        DICE_Y - DICE_SIZE / 2,
        DICE_SIZE,
        DICE_SIZE
    };
}

void LoadRenderer(Renderer& renderer)
{
    renderer.player1 = LoadTexture("assets/images/player1.png");
    renderer.player2 = LoadTexture("assets/images/player2.png");

    renderer.background = LoadTexture("assets/images/ui/background.png");
    renderer.diceSheet = LoadTexture("assets/images/ui/dice_faces.png");
    renderer.labelP1 = LoadTexture("assets/images/ui/label_p1.png");
    renderer.labelP2 = LoadTexture("assets/images/ui/label_p2.png");
    renderer.settingsIcon = LoadTexture("assets/images/ui/settings.png");

    SetTextureFilter(renderer.player1, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(renderer.player2, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(renderer.diceSheet, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(renderer.labelP1, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(renderer.labelP2, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(renderer.settingsIcon, TEXTURE_FILTER_BILINEAR);

    for (int i = 0; i < 9; i++)
    {
        renderer.snakeTex[i] = LoadTexture(SNAKE_DEFS[i].file);
        SetTextureFilter(renderer.snakeTex[i], TEXTURE_FILTER_BILINEAR);
    }

    for (int i = 0; i < 8; i++)
    {
        renderer.ladderTex[i] = LoadTexture(LADDER_DEFS[i].file);
        SetTextureFilter(renderer.ladderTex[i], TEXTURE_FILTER_BILINEAR);
    }
}

void DrawRenderer(
    const Renderer& renderer,
    int diceFace1,
    int diceFace2,
    Vector2 pawn1Position,
    Vector2 pawn2Position
)
{
    ClearBackground(RAYWHITE);

    DrawTexture(renderer.background, 0, 0, WHITE);

    DrawBoard();

    for (int i = 0; i < 8; i++)
        DrawBetweenCells(LADDER_DEFS[i], renderer.ladderTex[i]);

    for (int i = 0; i < 9; i++)
        DrawBetweenCells(SNAKE_DEFS[i], renderer.snakeTex[i]);

    DrawDice(renderer.diceSheet, diceFace1, DICE1_X, DICE_Y);
    DrawLabel(renderer.labelP1, DICE1_X, DICE_Y + DICE_SIZE / 2 + 8);

    DrawDice(renderer.diceSheet, diceFace2, DICE2_X, DICE_Y);
    DrawLabel(renderer.labelP2, DICE2_X, DICE_Y + DICE_SIZE / 2 + 8);

    // Pawns are drawn at their animated positions
    Vector2 p1 = pawn1Position;
    Vector2 p2 = pawn2Position;

    // When both pawns are on the same cell, slide them apart a little
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    float closeness = (PAWN_SPREAD_RANGE - distance) / PAWN_SPREAD_RANGE;
    if (closeness < 0.0f) closeness = 0.0f;
    if (closeness > 1.0f) closeness = 1.0f;

    p1.x -= PAWN_SPREAD * closeness;
    p2.x += PAWN_SPREAD * closeness;

    // The pawn that is lower on the screen is drawn last, so it appears in front
    if (p1.y <= p2.y)
    {
        DrawPawn(renderer.player1, p1);
        DrawPawn(renderer.player2, p2);
    }
    else
    {
        DrawPawn(renderer.player2, p2);
        DrawPawn(renderer.player1, p1);
    }

    DrawTextureEx(
        renderer.settingsIcon,
        {724, 16},
        0.0f,
        56.0f / renderer.settingsIcon.width,
        WHITE
    );
}

void UnloadRenderer(Renderer& renderer)
{
    UnloadTexture(renderer.player1);
    UnloadTexture(renderer.player2);

    UnloadTexture(renderer.background);
    UnloadTexture(renderer.diceSheet);
    UnloadTexture(renderer.labelP1);
    UnloadTexture(renderer.labelP2);
    UnloadTexture(renderer.settingsIcon);

    for (int i = 0; i < 9; i++)
        UnloadTexture(renderer.snakeTex[i]);

    for (int i = 0; i < 8; i++)
        UnloadTexture(renderer.ladderTex[i]);
}
