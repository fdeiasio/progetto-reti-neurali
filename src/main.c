#include <stdio.h>
#include <math.h>

#include "raylib.h"
#include "raymath.h"
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

static Lander lander = {
    .position = {LANDER_INIT_X, LANDER_INIT_Y, LANDER_INIT_Z},
    .velocity = {0, 0, 0},
    .fuel = LANDER_INIT_FUEL,
    .status = IDLE,
};

static Assets assets;

int main(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(MAX_FPS);

    LoadAssets(&assets);

    // Game loop
    while (!WindowShouldClose()) {
        ActionMask action = GetActionFromKeyboard();

        // Reset on game over or win
        if ((IsGameOver(lander.status) || IsSuccess(lander.status)) && IsKeyPressed(KEY_SPACE)) {
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
