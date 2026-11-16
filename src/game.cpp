#include "raylib.h"
#include "core.hpp"
#include "config.h"
#include "rendering.h"

static Input GetActionFromKeyboard() {
    Input input;
    input.left  = IsKeyDown(KEY_LEFT);
    input.right = IsKeyDown(KEY_RIGHT);
    input.space = IsKeyDown(KEY_SPACE);
    return input;
}

int main() {
    InitWindow(WORLD_WIDTH, WORLD_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(MAX_FPS);

    // Load game assets
    Assets assets;
    LoadAssets(&assets);

    // Instantiate lander
    Lander lander(LANDER_INIT_X, LANDER_INIT_Y);

    // Game loop
    while (!WindowShouldClose()) {
        Input action = GetActionFromKeyboard();

        // Reset on game over or win
        if (IsFinished(lander.status) && IsKeyPressed(KEY_SPACE)) {
            lander = Lander(LANDER_INIT_X, LANDER_INIT_Y);
            action = Input{};
        }

        lander.Update(action);

        BeginDrawing();
            RenderScene(&lander, &assets);
        EndDrawing();
    }

    UnloadAssets(&assets);

    CloseWindow();
}
