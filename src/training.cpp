#include "core.hpp"
#include "config.h"
#include "agent.h"

#define MAX_EPOCHS 1000

int main() {
    Lander lander(LANDER_INIT_X, LANDER_INIT_Y);

    // Training loop
    for (int epoch = 0; epoch < MAX_EPOCHS; epoch++) {
        // 1. Give the state to the agent and return the input

        // 2. Apply reward function for the training

        // 3. Verify win/loss and reset if needed
    }
}
