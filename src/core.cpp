#include <cmath>

#include "core.hpp"

namespace {
constexpr float kDegToRad = 3.14159265358979f / 180.0f;
}

Lander RandomLander(std::mt19937& rng) {
    const float x = std::uniform_real_distribution<float>{kLanderMinX, kLanderMaxX}(rng);
    const float y = std::uniform_real_distribution<float>{kLanderMinY, kLanderMaxY}(rng);
    return Lander{x, y};
}

bool Lander::IsOutOfBounds() const {
    return x < 0 || x > kWorldWidth || y < 0;
}

void Lander::Update(Input input) {
    if (IsFinished(status))
        return;

    UpdatePosition(input);
    UpdateFuel(input);
    UpdateStatus(input);
}

void Lander::UpdatePosition(Input input) {
    if (input.left)
        theta -= kTorque;
    if (input.right)
        theta += kTorque;

    // Keep the angle in [-180, 180] so a full turn counts as upright
    theta = std::remainder(theta, 360.0f);

    float ax = 0;
    float ay = kGravity;
    if (input.thrust) {
        const float radians = theta * kDegToRad;
        ax += kThrust * std::sin(radians);
        ay -= kThrust * std::cos(radians);
    }

    vx += ax * kDeltaTime;
    vy += ay * kDeltaTime;
    x  += vx * kDeltaTime;
    y  += vy * kDeltaTime;
}

void Lander::UpdateFuel(Input input) {
    if (input.thrust && fuel > 0)
        --fuel;
}

void Lander::UpdateStatus(Input input) {
    // Each new status check overrides the previous one,
    // so they are ordered by increasing priority
    Status newStatus = input.thrust ? Status::Active : Status::Idle;

    if (fuel == 0)
        newStatus = Status::Out_of_Fuel;

    // Touchdown: check for possible reasons for game over, else the player won
    if (y > kGroundHeight) {
        y = kGroundHeight;
        if (std::hypot(vx, vy) >= kVelThreshold)
            newStatus = Status::Too_Fast;
        else if (std::fabs(theta) >= kAngleThreshold)
            newStatus = Status::Too_Angled;
        else if (x <= kWinStart || x >= kWinEnd)
            newStatus = Status::Too_Far;
        else
            newStatus = Status::Won;
    }

    if (IsOutOfBounds())
        newStatus = Status::Out_of_Bounds;

    status = newStatus;
}
