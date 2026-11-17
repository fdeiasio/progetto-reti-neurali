#include <algorithm>
#include <cmath>

#include "core.hpp"

namespace {

constexpr float kDegToRad = 3.14159265358979f / 180.0f;

// ==== PHYSICS ====

void Rotate(Lander& l, GameInput input) {
    if (input.left)
        l.theta -= kTorque;
    if (input.right)
        l.theta += kTorque;

    // Keep the angle in [-180, 180] so a full turn counts as upright
    l.theta = std::remainder(l.theta, 360.0f);
}

void Move(Lander& l, GameInput input) {
    const float thrust  = input.thrust && l.fuel > 0 ? kThrust : 0;
    const float radians = l.theta * kDegToRad;
    const float ax = thrust * std::sin(radians);
    const float ay = kGravity - thrust * std::cos(radians);

    l.vx += ax * kDeltaTime;
    l.vy += ay * kDeltaTime;

    if (l.vy >  kMaxSpeed) { l.vy =  kMaxSpeed; }
    if (l.vy < -kMaxSpeed) { l.vy = -kMaxSpeed; }
    if (l.vx >  kMaxSpeed) { l.vx =  kMaxSpeed; }
    if (l.vx < -kMaxSpeed) { l.vx = -kMaxSpeed; }
    
    l.x  += l.vx * kDeltaTime;
    l.y  += l.vy * kDeltaTime;

    // Rest on the ground instead of sinking into it
    l.y = std::min(l.y, kGroundHeight);
}

void UpdateFuel(Lander& l, GameInput input) {
    if (input.thrust && l.fuel > 0)
        --l.fuel;
}

// ==== STATUS ====

bool IsOutOfBounds(const Lander& l) { return l.x < 0 || l.x > kWorldWidth || l.y < 0; }
bool HasTouchedDown(const Lander& l) { return l.y >= kGroundHeight; }
bool IsTooFast(const Lander& l)      { return std::hypot(l.vx, l.vy) >= kVelThreshold; }
bool IsTooAngled(const Lander& l)    { return std::fabs(l.theta) >= kAngleThreshold; }
bool IsOnPad(const Lander& l)        { return l.x > kWinStart && l.x < kWinEnd; }

Status TouchdownStatus(const Lander& l) {
    if (IsTooFast(l))
        return Status::Too_Fast;
    if (IsTooAngled(l))
        return Status::Too_Angled;
    if (!IsOnPad(l))
        return Status::Too_Far;
    return Status::Won;
}

// Checks run from highest to lowest priority
Status NextStatus(const Lander& l, GameInput input) {
    if (IsOutOfBounds(l))
        return Status::Out_of_Bounds;
    if (HasTouchedDown(l))
        return TouchdownStatus(l);
    return input.thrust && l.fuel > 0 ? Status::Active : Status::Idle;
}

}

Lander RandomLander(std::mt19937& rng) {
    const float x = std::uniform_real_distribution<float>{kLanderMinX, kLanderMaxX}(rng);
    const float y = std::uniform_real_distribution<float>{kLanderMinY, kLanderMaxY}(rng);
    return Lander{x, y};
}

void Lander::Update(GameInput input) {
    if (IsFinished(status))
        return;

    Rotate(*this, input);
    Move(*this, input);
    UpdateFuel(*this, input);
    status = NextStatus(*this, input);
}
