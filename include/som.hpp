#pragma once

#include <array>
#include <cstdint>
#include <random>

#include "config.hpp"
#include "core.hpp"

// ==== STATE VECTOR ====
struct StateVector {
    float x     = 0;
    float y     = 0;
    float vx    = 0;
    float vy    = 0;
    float theta = 0;

    StateVector() = default;

    explicit StateVector(const Lander& l);
};

// ==== SOM ====
using Weight = StateVector;

class SelfOrganizingMap {
public:
    std::array<Weight, kSomDim * kSomDim> weights{};

    explicit SelfOrganizingMap(std::mt19937& rng);

    [[nodiscard]] uint32_t GetWinner(const StateVector& input) const;

    uint32_t Update(const StateVector& input, uint32_t t);
};
