#include "raylib.h"
#include "core.hpp"
#include "rendering.hpp"

namespace {

class KeyboardController : public Controller {
public:
    [[nodiscard]] Input GetInput(const Lander&) override {
        return {IsKeyDown(KEY_LEFT), IsKeyDown(KEY_RIGHT), IsKeyDown(KEY_SPACE)};
    }
};

static const KeyboardController controller;

}  // namespace

int main() {
    const Renderer renderer;

    std::mt19937 rng{kLanderSeed};
    Lander lander = RandomLander(rng);

    // Game loop
    while (!renderer.ShouldClose()) {
        Input input = controller.GetInput(lander);

        // Reset on game over or win
        if (IsFinished(lander.status) && IsKeyPressed(KEY_SPACE)) {
            lander = RandomLander(rng);
            input  = Input{};
        }

        lander.Update(input);

        renderer.Draw(lander);
    }
}
