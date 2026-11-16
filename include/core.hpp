#pragma once

#include <cstdint>

// ==== LANDER STATUS ====
enum class Status {
    Idle,
    Active,
    Too_Fast,
    Too_Angled,
    Too_Far,
    Out_of_Bounds,
    Out_of_Fuel,
    Won
};

inline constexpr bool IsSuccess(Status s)  { return s == Status::Won; }
inline constexpr bool IsFinished(Status s) { return s != Status::Idle && s != Status::Active; }
inline constexpr bool IsGameOver(Status s) { return IsFinished(s) && !IsSuccess(s); }

// ==== INPUT ====
struct Input {
    bool left  = 0;
    bool right = 0;
    bool space = 0;
};

// ==== LANDER ====
inline constexpr uint32_t kStartingFuel = 800;

struct Lander {
    float       x       = 0;
    float       y       = 0;
    float       vx      = 0;
    float       vy      = 0;
    float       tetha   = 0;
    uint32_t    fuel    = kStartingFuel;
    Status      status  = Status::Idle;

    Lander(float x0, float y0) : x(x0), y(y0) {}

    void Update(Input input);
};
