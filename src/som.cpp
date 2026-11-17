#include <algorithm>
#include <cmath>
#include <limits>

#include "som.hpp"

// ==== STATE VECTOR ====

StateVector::StateVector(const Lander& l)
    : x    (std::clamp(l.x / kWorldWidth, 0.0f, 1.0f))
    , y    (std::clamp(l.y / kGroundHeight, 0.0f, 1.0f))
    , vx   (std::tanh(l.vx / (2 * kVelThreshold)))
    , vy   (std::tanh(l.vy / (2 * kVelThreshold)))
    , theta(std::tanh(l.theta / (2 * kAngleThreshold)))
{}

// ==== SCHEDULES ====

float kSomAlpha(uint32_t t)  { return std::max(kSomAlphaMin,  kSomAlphaStart  * std::exp(-static_cast<float>(t) / kSomDecaySteps)); }
float kSomRadius(uint32_t t, float start) { return std::max(kSomRadiusMin, start * std::exp(-static_cast<float>(t) / kSomDecaySteps)); }

// ==== SOM ====

SelfOrganizingMap::SelfOrganizingMap(std::mt19937& rng) {
    std::uniform_real_distribution<float> unit(0, 1);
    std::uniform_real_distribution<float> signed_(-1, 1);

    for (Weight& w : weights) {
        w.x     = unit(rng);
        w.y     = unit(rng);
        w.vx    = signed_(rng);
        w.vy    = signed_(rng);
        w.theta = signed_(rng);
    }
}

uint32_t SelfOrganizingMap::GetWinner(const StateVector& input) const {
    uint32_t winner  = 0;
    float    minDist = std::numeric_limits<float>::max();

    for (uint32_t i = 0; i < weights.size(); i++) {
        const Weight& w = weights[i];
        const float dx     = w.x     - input.x;
        const float dy     = w.y     - input.y;
        const float dvx    = w.vx    - input.vx;
        const float dvy    = w.vy    - input.vy;
        const float dtheta = w.theta - input.theta;
        const float dist   = dx * dx + dy * dy + dvx * dvx + dvy * dvy + dtheta * dtheta;

        if (dist < minDist) {
            minDist = dist;
            winner  = i;
        }
    }
    return winner;
}

uint32_t SelfOrganizingMap::Update(const StateVector& input, uint32_t t) {
    const uint32_t winner = GetWinner(input);
    const int      wRow   = static_cast<int>(winner) / kSomDim;
    const int      wCol   = static_cast<int>(winner) % kSomDim;
    const float    alpha  = kSomAlpha(t);
    const float    r2     = kSomRadius(t, kSomDim / 2.0f) * kSomRadius(t, kSomDim / 2.0f);

    const auto h = [r2](float d2) { return std::exp(-d2 / (2 * r2)); };

    for (uint32_t i = 0; i < weights.size(); i++) {
        const int   dRow = static_cast<int>(i) / kSomDim - wRow;
        const int   dCol = static_cast<int>(i) % kSomDim - wCol;
        const float rate = alpha * h(static_cast<float>(dRow * dRow + dCol * dCol));

        Weight& w = weights[i];
        w.x     += rate * (input.x     - w.x);
        w.y     += rate * (input.y     - w.y);
        w.vx    += rate * (input.vx    - w.vx);
        w.vy    += rate * (input.vy    - w.vy);
        w.theta += rate * (input.theta - w.theta);
    }
    return winner;
}