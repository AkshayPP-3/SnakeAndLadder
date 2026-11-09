
#include "movement.hpp"

Movement::Movement()
{
    startPosition = {0, 0};
    targetPosition = {0, 0};
    currentPosition = {0, 0};

    duration = 0.8f;
    timer = 0.0f;

    moving = false;
}

void Movement::start(Vector2 start, Vector2 target, float moveDuration)
{
    startPosition = start;
    targetPosition = target;
    currentPosition = start;

    duration = moveDuration;
    timer = 0.0f;

    moving = true;
}

void Movement::update(float deltaTime)
{
    if (!moving)
        return;

    timer += deltaTime;

    float progress = timer / duration;

    if (progress >= 1.0f)
    {
        progress = 1.0f;
        moving = false;
    }

    // Smooth ease-in-out interpolation
    float smoothProgress = progress * progress * (3.0f - 2.0f * progress);

    currentPosition.x = startPosition.x +
        (targetPosition.x - startPosition.x) * smoothProgress;

    currentPosition.y = startPosition.y +
        (targetPosition.y - startPosition.y) * smoothProgress;
}

bool Movement::isMoving() const
{
    return moving;
}

Vector2 Movement::getPosition() const
{
    return currentPosition;
}
