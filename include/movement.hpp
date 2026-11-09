
#pragma once

#include "raylib.h"

class Movement
{
private:
    Vector2 startPosition;
    Vector2 targetPosition;
    Vector2 currentPosition;

    float duration;
    float timer;

    bool moving;

public:
    Movement();

    void start(Vector2 start, Vector2 target, float moveDuration = 0.8f);
    void update(float deltaTime);

    bool isMoving() const;
    Vector2 getPosition() const;
};
