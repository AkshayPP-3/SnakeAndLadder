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

struct SnakeDef
{
    int head;              // cell where the snake's head is
    int tail;              // cell where the snake's tail ends
    const char *file;      // image of this one snake
    Vector2 headPoint;     // face center inside the image (pixels)
    Vector2 tailPoint;     // tail tip inside the image (pixels)
};

// snakes[0] .. snakes[8]  ->  {head, tail} as you specified
const SnakeDef SNAKE_DEFS[9] = {
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

// Draw one snake so its head sits on the head cell and its tail tip on the tail cell
void DrawSnake(const SnakeDef &s, Texture2D tex)
{
    Vector2 h = CellCenter(s.head);
    Vector2 t = CellCenter(s.tail);

    float bx = t.x - h.x, by = t.y - h.y;                       // board vector
    float sx = s.tailPoint.x - s.headPoint.x;                   // image vector
    float sy = s.tailPoint.y - s.headPoint.y;

    float scale    = std::sqrt(bx * bx + by * by) / std::sqrt(sx * sx + sy * sy);
    float rotation = (std::atan2(by, bx) - std::atan2(sy, sx)) * RAD2DEG;

    Rectangle src  = {0, 0, (float)tex.width, (float)tex.height};
    Rectangle dest = {h.x, h.y, tex.width * scale, tex.height * scale};
    Vector2 origin = {s.headPoint.x * scale, s.headPoint.y * scale};  // rotate around the head

    DrawTexturePro(tex, src, dest, origin, rotation, WHITE);
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

    Texture2D snakeTex[9];
    for (int i = 0; i < 9; i++)
    {
        snakeTex[i] = LoadTexture(SNAKE_DEFS[i].file);
        SetTextureFilter(snakeTex[i], TEXTURE_FILTER_BILINEAR);
    }

    Game game;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawBoard();

        // Snakes
        for (int i = 0; i < 9; i++)
            DrawSnake(SNAKE_DEFS[i], snakeTex[i]);

        // Players outside the board (left side), one cell in size
        float p1Scale = (float)CELL / player1.height;
        float p2Scale = (float)CELL / player2.height;
        DrawTextureEx(player1, {90, 480}, 0.0f, p1Scale, WHITE);
        DrawTextureEx(player2, {90, 530}, 0.0f, p2Scale, WHITE);

        EndDrawing();
    }

    // Release image memory
    UnloadTexture(player1);
    UnloadTexture(player2);
    for (int i = 0; i < 9; i++) UnloadTexture(snakeTex[i]);

    CloseWindow();

    return 0;
}