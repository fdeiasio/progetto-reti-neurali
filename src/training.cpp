#include "core.hpp"
#include "agent.hpp"

constexpr int kMaxEpochs = 1000;

int main() {
    [[maybe_unused]] Lander lander;

    // Training loop
    for (int epoch = 0; epoch < kMaxEpochs; epoch++) {
        // 1. Give the state to the agent and return the input

        // 2. Apply reward function for the training

        // 3. Verify win/loss and reset if needed
    }
}
