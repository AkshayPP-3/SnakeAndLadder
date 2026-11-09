
#pragma once

#include "raylib.h"
#include "game.hpp"

#include <vector>

// One leg of a pawn's journey
struct MoveStep
{
    Vector2 target;      // where this leg ends
    float duration;      // how long this leg takes (seconds)
    float hopHeight;     // 0 = slide, > 0 = small jump in pixels
    float pauseBefore;   // wait this long before the leg starts (seconds)
};

class Movement
{
private:
    Vector2 startPosition;
    Vector2 targetPosition;
    Vector2 currentPosition;

    float duration;
    float timer;
    float hopHeight;

    bool moving;

    std::vector<MoveStep> steps;
    size_t stepIndex;

    void beginStep();

public:
    Movement();

    // Put the pawn somewhere instantly (no animation)
    void setPosition(Vector2 position);

    // One straight move (kept from before)
    void start(Vector2 start, Vector2 target, float moveDuration = 0.8f);

    // Queue several legs, then call begin() to play them one after another
    void addStep(Vector2 target, float stepDuration, float stepHopHeight = 0.0f, float pauseBefore = 0.0f);
    void begin();

    void update(float deltaTime);

    bool isMoving() const;
    Vector2 getPosition() const;
};

// Turns a finished dice roll into the pawn animation:
//   outside -> cell 1, cell by cell, then slide along a snake / ladder
void PlanPawnMove(Movement& movement, const TurnResult& result);
