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

int main(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(MAX_FPS);

    // Load assets
    Texture2D bg = LoadTexture("assets/bg.jpg");

    Texture2D sprites[SPRITE_COUNT];
    sprites[SPRITE_IDLE]    = LoadTexture("assets/sprite_0.png");
    sprites[SPRITE_THRUST_1]= LoadTexture("assets/sprite_1.png");
    sprites[SPRITE_THRUST_2]= LoadTexture("assets/sprite_2.png");
    sprites[SPRITE_CRASHED] = LoadTexture("assets/sprite_3.png");

    Font font = LoadFontEx("assets/font.ttf", 32, 0, 250);

    // Game loop
    while (!WindowShouldClose()) {
        ActionMask action = GetActionFromKeyboard();

        // Reset on game over or win
        if ((IsGameOver(lander.status) || IsSuccess(lander.status)) && IsKeyPressed(KEY_SPACE)) {
            ResetLander(&lander);
            action = 0;
        }

        // Update game state
        UpdateLander(&lander, action);

        // Draw
        BeginDrawing();
            DrawBackground(bg);
            DrawFuelDisplay(font, lander.fuel);
            DrawLander(&lander, sprites);

            if (IsGameOver(lander.status))
                DrawGameOverScreen(font, lander.status);

            if (IsSuccess(lander.status))
                DrawWinScreen(font);
        EndDrawing();
    }

    // Cleanup
    UnloadTexture(bg);
    UnloadTexture(sprites[SPRITE_IDLE]);
    UnloadTexture(sprites[SPRITE_THRUST_1]);
    UnloadTexture(sprites[SPRITE_THRUST_2]);
    UnloadTexture(sprites[SPRITE_CRASHED]);
    UnloadFont(font);
    CloseWindow();
}
