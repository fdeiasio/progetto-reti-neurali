#include "raylib.h"
#include "core.h"
#include "rendering.h"

static ActionMask GetActionFromKeyboard(void) {
    ActionMask input = 0;

    if (IsKeyDown(KEY_LEFT))
        input |= ACTION_LEFT;
    if (IsKeyDown(KEY_RIGHT))
        input |= ACTION_RIGHT;
    if (IsKeyDown(KEY_SPACE))
        input |= ACTION_THRUST;

    return input;
}

int main(void) {
    InitWindow(WORLD_WIDTH, WORLD_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(MAX_FPS);

    // Load game assets
    Assets assets;
    LoadAssets(&assets);

    // Instantiate lander
    Lander lander;
    ResetLander(&lander);

    // Game loop
    while (!WindowShouldClose()) {
        ActionMask action = GetActionFromKeyboard();

        // Reset on game over or win
        if (IsFinished(lander.status) && IsKeyPressed(KEY_SPACE)) {
            ResetLander(&lander);
            action = 0;
        }

        UpdateLander(&lander, action);

        BeginDrawing();
            RenderScene(&lander, &assets);
        EndDrawing();
    }

    UnloadAssets(&assets);

    CloseWindow();
}
