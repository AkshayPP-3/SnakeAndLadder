
#include "renderer.hpp"
#include "board.hpp"

#include <cmath>
#include <cstdlib>

const int BOARD_X = 150;
const int BOARD_Y = 50;
const int CELL = 50;

const float DICE_SIZE = 80.0f;
const float DICE_Y = 290.0f;
const float DICE1_X = 79.0f;
const float DICE2_X = 721.0f;

const float PAWN_HEIGHT       = 40.0f;
const float PAWN_FOOT         = 18.0f;
const float PAWN_SPREAD       = 8.0f;
const float PAWN_SPREAD_RANGE = 30.0f;

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

    // Font for the winning banner (falls back to raylib's default font)
    renderer.winFont = GetFontDefault();

    const char* fontFiles[] = {
        "C:/Windows/Fonts/segoeuib.ttf",
        "C:/Windows/Fonts/arialbd.ttf"
    };

    for (const char* file : fontFiles)
    {
        if (FileExists(file))
        {
            renderer.winFont = LoadFontEx(file, 96, nullptr, 0);
            break;
        }
    }

    SetTextureFilter(renderer.winFont.texture, TEXTURE_FILTER_BILINEAR);

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

    if (renderer.winFont.texture.id != GetFontDefault().texture.id)
        UnloadFont(renderer.winFont);

    for (int i = 0; i < 9; i++)
        UnloadTexture(renderer.snakeTex[i]);

    for (int i = 0; i < 8; i++)
        UnloadTexture(renderer.ladderTex[i]);
}


// ---------- Winning animation ----------

static float RandomFloat(float minValue, float maxValue)
{
    return minValue + (maxValue - minValue) * ((float)rand() / (float)RAND_MAX);
}

static float Clamp01(float value)
{
    if (value < 0.0f) return 0.0f;
    if (value > 1.0f) return 1.0f;
    return value;
}

// Goes a little past 1 and settles back (gives the "pop" when things appear)
static float EaseOutBack(float t)
{
    const float c1 = 1.70158f;
    const float c3 = c1 + 1.0f;
    float u = t - 1.0f;

    return 1.0f + c3 * u * u * u + c1 * u * u;
}

static Rectangle GetPlayAgainButton()
{
    return {300, 345, 200, 52};
}

void StartWinAnimation(WinAnimation& win, int winner)
{
    win.active = true;
    win.winner = winner;
    win.timer = 0.0f;
    win.confetti.clear();

    const Color palette[] = {
        {232, 72, 72, 255},
        {255, 201, 51, 255},
        {88, 196, 95, 255},
        {66, 153, 225, 255},
        {237, 100, 166, 255},
        {155, 100, 230, 255},
        {247, 150, 50, 255}
    };

    for (int i = 0; i < 150; i++)
    {
        Confetti piece;

        // start above the screen, so the confetti pours in
        piece.position = {RandomFloat(0, 800), RandomFloat(-700, -10)};
        piece.speed = RandomFloat(90, 220);
        piece.swayAmount = RandomFloat(20, 60);
        piece.swayPhase = RandomFloat(0, 6.28f);
        piece.swaySpeed = RandomFloat(2, 5);
        piece.rotation = RandomFloat(0, 360);
        piece.rotationSpeed = RandomFloat(-300, 300);
        piece.width = RandomFloat(8, 14);
        piece.height = RandomFloat(5, 9);
        piece.color = palette[rand() % 7];

        win.confetti.push_back(piece);
    }
}

void UpdateWinAnimation(WinAnimation& win, float deltaTime)
{
    if (!win.active)
        return;

    win.timer += deltaTime;

    for (Confetti& piece : win.confetti)
    {
        piece.position.y += piece.speed * deltaTime;

        piece.swayPhase += piece.swaySpeed * deltaTime;
        piece.position.x += std::cos(piece.swayPhase) * piece.swayAmount * deltaTime;

        piece.rotation += piece.rotationSpeed * deltaTime;

        // fell off the bottom: send it back to the top
        if (piece.position.y > 620.0f)
        {
            piece.position.y = -RandomFloat(10, 60);
            piece.position.x = RandomFloat(0, 800);
        }
    }
}

bool IsPlayAgainPressed(const WinAnimation& win)
{
    // small delay so the click that finished the game can't trigger it
    if (!win.active || win.timer < 1.0f)
        return false;

    if (IsKeyPressed(KEY_ENTER))
        return true;

    return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
           CheckCollisionPointRec(GetMousePosition(), GetPlayAgainButton());
}

// Text with a soft shadow, centered at (cx, cy)
static void DrawCenteredText(const Font& font, const char* text, float cx, float cy,
                             float size, float spacing, Color color, Color shadow)
{
    Vector2 measured = MeasureTextEx(font, text, size, spacing);
    Vector2 pos = {cx - measured.x / 2, cy - measured.y / 2};

    DrawTextEx(font, text, {pos.x, pos.y + size * 0.06f}, size, spacing, shadow);
    DrawTextEx(font, text, pos, size, spacing, color);
}

void DrawWinAnimation(const Renderer& renderer, const WinAnimation& win)
{
    if (!win.active)
        return;

    const float t = win.timer;
    const Vector2 center = {400, 250};

    // 1. Dim the whole screen
    float fade = Clamp01(t / 0.5f);
    DrawRectangle(0, 0, 800, 600, Fade({10, 20, 50, 255}, 0.55f * fade));

    // 2. Slowly turning golden rays and a warm glow behind the winner
    const int rays = 14;
    const float step = 2.0f * PI / rays;
    float turn = t * 0.35f;

    for (int i = 0; i < rays; i++)
    {
        float a0 = turn + i * step;
        float a1 = a0 + step * 0.5f;

        Vector2 p0 = {center.x + std::cos(a0) * 560.0f, center.y + std::sin(a0) * 560.0f};
        Vector2 p1 = {center.x + std::cos(a1) * 560.0f, center.y + std::sin(a1) * 560.0f};

        DrawTriangle(center, p1, p0, Fade({255, 214, 64, 255}, 0.20f * fade));
    }

    // soft glow: several see-through circles stacked on top of each other
    for (int i = 0; i < 10; i++)
    {
        float radius = 230.0f * (1.0f - i / 10.0f);
        DrawCircleV(center, radius, Fade({255, 226, 120, 255}, 0.07f * fade));
    }

    // 3. Confetti
    for (const Confetti& piece : win.confetti)
    {
        Rectangle rect = {piece.position.x, piece.position.y, piece.width, piece.height};

        DrawRectanglePro(rect, {piece.width / 2, piece.height / 2}, piece.rotation, piece.color);
    }

    // 4. Winner pawn: pops in, then hops up and down
    Texture2D pawn = (win.winner == 1) ? renderer.player1 : renderer.player2;

    float pawnPop = EaseOutBack(Clamp01(t / 0.7f));
    float pawnHeight = 110.0f * pawnPop;
    float hop = (t > 0.7f) ? std::fabs(std::sin((t - 0.7f) * 4.0f)) * 14.0f : 0.0f;

    float groundY = 218.0f;
    float pawnScale = pawnHeight / pawn.height;

    DrawEllipse(400, (int)groundY + 4, 30.0f - hop * 0.8f, 7.0f - hop * 0.2f, Fade(BLACK, 0.30f));

    DrawTextureEx(
        pawn,
        {400 - pawn.width * pawnScale / 2, groundY - hop - pawnHeight},
        0.0f,
        pawnScale,
        WHITE
    );

    // 5. Banner: pops in, then breathes a little
    float bannerScale = EaseOutBack(Clamp01((t - 0.25f) / 0.6f));
    if (t > 0.9f)
        bannerScale *= 1.0f + 0.02f * std::sin(t * 3.5f);

    if (bannerScale > 0.05f)
    {
        float w = 430.0f * bannerScale;
        float h = 92.0f * bannerScale;
        Rectangle banner = {400 - w / 2, 285 - h / 2, w, h};

        DrawRectangleRounded({banner.x, banner.y + 7, w, h}, 0.5f, 12, Fade(BLACK, 0.30f));
        DrawRectangleRounded(banner, 0.5f, 12, {95, 50, 18, 255});

        Rectangle inner = {banner.x + 5, banner.y + 5, w - 10, h - 10};
        DrawRectangleRounded(inner, 0.5f, 12, {150, 85, 35, 255});

        Rectangle shine = {inner.x + 4, inner.y + 3, inner.width - 8, inner.height * 0.45f};
        DrawRectangleRounded(shine, 0.5f, 12, Fade({255, 200, 120, 255}, 0.22f));

        DrawCenteredText(
            renderer.winFont,
            TextFormat("Player %d wins!", win.winner),
            400, 285,
            50.0f * bannerScale, 2.0f * bannerScale,
            {255, 244, 220, 255},
            {70, 35, 10, 255}
        );
    }

    // 6. "Play again" button fades in after the banner
    float buttonAlpha = Clamp01((t - 0.9f) / 0.4f);

    if (buttonAlpha > 0.0f)
    {
        Rectangle button = GetPlayAgainButton();
        bool hover = CheckCollisionPointRec(GetMousePosition(), button);

        Color border = {40, 110, 30, 255};
        Color fill = hover ? Color{112, 214, 82, 255} : Color{88, 190, 60, 255};

        DrawRectangleRounded({button.x, button.y + 5, button.width, button.height}, 0.5f, 12, Fade(BLACK, 0.30f * buttonAlpha));
        DrawRectangleRounded(button, 0.5f, 12, Fade(border, buttonAlpha));
        DrawRectangleRounded({button.x + 4, button.y + 4, button.width - 8, button.height - 8}, 0.5f, 12, Fade(fill, buttonAlpha));

        DrawCenteredText(
            renderer.winFont, "Play again",
            button.x + button.width / 2, button.y + button.height / 2,
            26.0f, 1.0f,
            Fade(WHITE, buttonAlpha),
            Fade({30, 80, 20, 255}, buttonAlpha)
        );
    }
}
