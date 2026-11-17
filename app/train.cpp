#include <cstdint>

#include "core.hpp"
#include "agent.hpp"

inline constexpr uint32_t kSeed = 42;

static const KeyboardController controller;

int main() {
    for (int episode = 0; episode < kMaxEpisodes; episode++) {
        // Episode e starts from seed kBaseSeed + e
        Lander lander = RandomLander(kSeed + static_cast<std::uint32_t>(episode));

        // A finished episode simply ends this loop, and the next one spawns
        while (!IsFinished(lander.status)) {
            // 1. Give the state to the agent and get the action

            // 2. Apply the reward function for the training

            break;  // placeholder until the agent exists (plan step 7)
        }
    }
}
