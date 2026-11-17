#pragma once

#include <cstdint>
#include <random>

#include "config.hpp"

// ==== LANDER STATUS ====
enum class Status {
    Idle,
    Active,
    Too_Fast,
    Too_Angled,
    Too_Far,
    Out_of_Bounds,
    Won
};

constexpr bool IsSuccess(Status s)  { return s == Status::Won; }
constexpr bool IsFinished(Status s) { return s != Status::Idle && s != Status::Active; }
constexpr bool IsGameOver(Status s) { return IsFinished(s) && !IsSuccess(s); }

// ==== INPUT ====
struct GameInput {
    bool left   = false;
    bool right  = false;
    bool thrust = false;
};

// ==== LANDER ====
class Lander {
public:
    float    x      = 0;
    float    y      = 0;
    float    vx     = 0;
    float    vy     = 0;
    float    theta  = kLanderInitAngle;
    uint32_t fuel   = kLanderInitFuel;
    Status   status = Status::Idle;

    explicit Lander(float x0, float y0) : x(x0), y(y0) {}

    void Update(GameInput input);
};

// Lander at a random point inside the start boundary; same seed, same sequence
Lander RandomLander(std::mt19937& rng);

// ==== CONTROLLER ====
// Whatever plays the game: the keyboard or the agent
class Controller {
public:
    virtual ~Controller() = default;
    [[nodiscard]] virtual GameInput GetInput(const Lander& lander) const = 0;
};
