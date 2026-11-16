#include <cmath>

#include "core.hpp"
#include "config.h"

static constexpr float kDeg2Rad = 3.14159265358979f / 180.0f;

static void UpdatePosition(Lander& l, Input input) {
    float ax = 0;
    float ay = GRAVITY;

    if (input.left)
        l.tetha -= TORQUE;
    if (input.right)
        l.tetha += TORQUE;

    // Keep the angle in [-180, 180] so a full turn counts as upright
    l.tetha = std::remainder(l.tetha, 360.0f);

    if (input.space) {
        float radians = l.tetha * kDeg2Rad;
        ax += THRUST * std::sin(radians);
        ay -= THRUST * std::cos(radians);
    }

    l.vx += ax * DELTA_TIME;
    l.vy += ay * DELTA_TIME;
    l.x  += l.vx * DELTA_TIME;
    l.y  += l.vy * DELTA_TIME;
}

static void UpdateFuel(Lander& l, Input input) {
    if (input.space && l.fuel > 0)
        l.fuel--;
}

static bool IsLanderOutOfBounds(const Lander& l) {
    return l.x < 0 || l.x > WORLD_WIDTH ||
           l.y < 0 || l.y > WORLD_HEIGHT;
}

static bool CheckGroundCollision(const Lander& l) {
    return l.y > GROUND_HEIGHT;
}

static Status ApplyGroundCollision(Lander& l) {
    float speed = std::hypot(l.vx, l.vy);

    l.y = GROUND_HEIGHT;

    // Check for possible reasons for game over
    if (speed >= VEL_THRESHOLD)
        return Status::Too_Fast;
    if (std::fabs(l.tetha) >= ANGLE_THRESHOLD)
        return Status::Too_Angled;
    if (l.x <= WIN_START || l.x >= WIN_END)
        return Status::Too_Far;

    // If none of it applies, the player must have won
    return Status::Won;
}

static void UpdateStatus(Lander& l, Input input) {
    // Each new status check overrides the previous one,
    // so they are ordered by increasing priority
    Status newStatus = Status::Idle;

    if (input.space)
        newStatus = Status::Active;

    if (l.fuel == 0)
        newStatus = Status::Out_of_Fuel;

    if (CheckGroundCollision(l))
        newStatus = ApplyGroundCollision(l);

    if (IsLanderOutOfBounds(l))
        newStatus = Status::Out_of_Bounds;

    l.status = newStatus;
}

void Lander::Update(Input input) {
    if (IsFinished(status))
        return;

    UpdatePosition(*this, input);
    UpdateFuel(*this, input);
    UpdateStatus(*this, input);
}
