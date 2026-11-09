
#include "movement.hpp"
#include "renderer.hpp"

#include <cmath>

Movement::Movement()
{
    startPosition = {0, 0};
    targetPosition = {0, 0};
    currentPosition = {0, 0};

    duration = 0.8f;
    timer = 0.0f;
    hopHeight = 0.0f;

    moving = false;
    stepIndex = 0;
}

void Movement::setPosition(Vector2 position)
{
    startPosition = position;
    targetPosition = position;
    currentPosition = position;

    steps.clear();
    stepIndex = 0;
    moving = false;
}

void Movement::start(Vector2 start, Vector2 target, float moveDuration)
{
    setPosition(start);

    addStep(target, moveDuration);
    begin();
}

void Movement::addStep(Vector2 target, float stepDuration, float stepHopHeight, float pauseBefore)
{
    steps.push_back({target, stepDuration, stepHopHeight, pauseBefore});
}

void Movement::begin()
{
    if (steps.empty())
        return;

    stepIndex = 0;
    beginStep();

    moving = true;
}

// Start the leg at stepIndex from wherever the pawn is now
void Movement::beginStep()
{
    const MoveStep& step = steps[stepIndex];

    startPosition = currentPosition;
    targetPosition = step.target;

    duration = (step.duration > 0.001f) ? step.duration : 0.001f;
    hopHeight = step.hopHeight;

    // A negative timer means "still waiting before this leg starts"
    timer = -step.pauseBefore;
}

void Movement::update(float deltaTime)
{
    if (!moving)
        return;

    timer += deltaTime;

    if (timer < 0.0f)
        return;

    float progress = timer / duration;
    bool legFinished = false;

    if (progress >= 1.0f)
    {
        progress = 1.0f;
        legFinished = true;
    }

    // Smooth ease-in-out interpolation
    float smoothProgress = progress * progress * (3.0f - 2.0f * progress);

    currentPosition.x = startPosition.x +
        (targetPosition.x - startPosition.x) * smoothProgress;

    // The hop lifts the pawn up and brings it back down on the target
    currentPosition.y = startPosition.y +
        (targetPosition.y - startPosition.y) * smoothProgress -
        hopHeight * std::sin(PI * progress);

    if (legFinished)
    {
        currentPosition = targetPosition;

        stepIndex++;

        if (stepIndex < steps.size())
        {
            beginStep();
        }
        else
        {
            moving = false;
            steps.clear();
            stepIndex = 0;
        }
    }
}

bool Movement::isMoving() const
{
    return moving;
}

Vector2 Movement::getPosition() const
{
    return currentPosition;
}

void PlanPawnMove(Movement& movement, const TurnResult& result)
{
    if (!result.moved)
        return;

    const float hopTime   = 0.28f;   // seconds per cell
    const float hopHeight = 12.0f;   // pixels

    if (result.from == 0)
    {
        // Got a 1: pawn travels from outside the board to cell 1
        movement.addStep(GetPawnAnchor(1), 0.9f, 40.0f);
    }
    else
    {
        // Normal move: one hop per cell
        for (int cell = result.from + 1; cell <= result.landed; cell++)
            movement.addStep(GetPawnAnchor(cell), hopTime, hopHeight);
    }

    // Snake head or ladder bottom: slide smoothly to the other end
    if (result.to != result.landed)
    {
        Vector2 a = GetPawnAnchor(result.landed);
        Vector2 b = GetPawnAnchor(result.to);

        float dx = b.x - a.x;
        float dy = b.y - a.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        // short pause on the cell first, longer slide for longer snakes / ladders
        movement.addStep(b, 0.6f + distance / 500.0f, 0.0f, 0.35f);
    }

    movement.begin();
}
