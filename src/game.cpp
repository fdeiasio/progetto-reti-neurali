#include "raylib.h"
#include "core.hpp"
#include "rendering.hpp"

namespace {

class KeyboardController : public Controller {
public:
    [[nodiscard]] Input GetInput(const Lander&) const override {
        return { IsKeyDown(KEY_LEFT), IsKeyDown(KEY_RIGHT), IsKeyDown(KEY_SPACE) };
    }
};

}  // namespace

static const KeyboardController controller;

int main() {
    const Renderer gameWindow;

    std::mt19937 rng{42};
    Lander lander = RandomLander(rng);

    // Game loop
    while (!gameWindow.ShouldClose()) {
        Input input = controller.GetInput(lander);

        // Reset on game over or win
        if (IsFinished(lander.status) && IsKeyPressed(KEY_SPACE)) [[unlikely]] {
            lander = RandomLander(rng);
            continue;
        }

        lander.Update(input);
        gameWindow.Draw(lander);
    }
}
