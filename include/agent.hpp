#pragma once

#include "core.hpp"

// State -> Kohonen network -> Q-matrix -> input
class Agent : public Controller {
public:
    [[nodiscard]] Input GetInput(const Lander& lander) override;
};
