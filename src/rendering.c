#include <stdio.h>

#include "raylib.h"
#include "rendering.h"
#include "core.h"

// ==================== ASSETS ===================

static const char* const SPRITE_PATHS[SPRITE_COUNT] = {
    [SPRITE_IDLE]     = "assets/sprite_0.png",
    [SPRITE_THRUST_1] = "assets/sprite_1.png",
    [SPRITE_THRUST_2] = "assets/sprite_2.png",
    [SPRITE_CRASHED]  = "assets/sprite_3.png",
};

void LoadAssets(Assets* assets) {
    assets->background = LoadTexture("assets/bg.jpg");
    for (int i = 0; i < SPRITE_COUNT; i++)
        assets->sprites[i] = LoadTexture(SPRITE_PATHS[i]);
    assets->font = LoadFontEx("assets/font.ttf", TEXT_FONT_SIZE, 0, 250);
}

void UnloadAssets(Assets* assets) {
    UnloadTexture(assets->background);
    for (int i = 0; i < SPRITE_COUNT; i++)
        UnloadTexture(assets->sprites[i]);
    UnloadFont(assets->font);
}

// ==================== RENDERING ==================

static SpriteIndex GetLanderSpriteIndex(const Lander* l) {
    switch (l->status) {
        case STATUS_ACTIVE:
            return (GetTime() * 4) - ((int)(GetTime() * 4)) < 0.5
                ? SPRITE_THRUST_1
                : SPRITE_THRUST_2;
        case STATUS_TOO_ANGLED:
        case STATUS_TOO_FAR:
        case STATUS_TOO_FAST:
            return SPRITE_CRASHED;
        case STATUS_IDLE:
        case STATUS_OUT_OF_FUEL:
        case STATUS_OUT_OF_BOUNDS:
        case STATUS_WON:
            return SPRITE_IDLE;
    }
    return SPRITE_IDLE;
}

static void DrawLander(const Lander* l, const Texture2D sprites[]) {
    Texture2D sprite = sprites[GetLanderSpriteIndex(l)];

    Rectangle src = {0, 0, sprite.width, sprite.height};
    Rectangle dst = {
        .x = l->position.x,
        .y = l->position.y,
        .width = LANDER_WIDTH,
        .height = LANDER_HEIGHT,
    };
    Vector2 origin = {LANDER_WIDTH / 2, LANDER_HEIGHT / 2};

    DrawTexturePro(sprite, src, dst, origin, l->position.z, WHITE);
}

static void DrawBackground(Texture2D bg) {
    Rectangle src = {0, 0, bg.width, bg.height};
    Rectangle dst = {0, 0, GetScreenWidth(), GetScreenHeight()};

    DrawTexturePro(bg, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
}

static const char* GetGameOverMessage(Status s) {
    switch (s) {
        case STATUS_OUT_OF_FUEL:
            return "The lander has been impacted by oil shortages";
        case STATUS_OUT_OF_BOUNDS:
            return "The lander had ambitions that exceeded its capabilities";
        case STATUS_TOO_FAST:
            return "The lander met the ground too fast for its own good";
        case STATUS_TOO_ANGLED:
            return "The lander tried to land on its side, it failed";
        case STATUS_TOO_FAR:
            return "The lander should calibrate distance better";
        default:
            return "Unknown";
    }
}

static void DrawEndScreen(Font font, const char* title, const char* subtitle) {
    Vector2 title_size = MeasureTextEx(font, title, TITLE_FONT_SIZE, 0);
    Vector2 title_pos = {
        GetScreenWidth() / 2 - title_size.x / 2,
        GetScreenHeight() / 2 - title_size.y / 2 - 100,
    };
    DrawTextEx(font, title, title_pos, TITLE_FONT_SIZE, 0, WHITE);

    Vector2 subtitle_size = MeasureTextEx(font, subtitle, TEXT_FONT_SIZE, 0);
    Vector2 subtitle_pos = {
        GetScreenWidth() / 2 - subtitle_size.x / 2,
        title_pos.y + title_size.y + 20,
    };
    DrawTextEx(font, subtitle, subtitle_pos, TEXT_FONT_SIZE, 0, WHITE);
}

static void DrawFuelDisplay(Font font, int32_t fuel) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "Fuel: %d", fuel);
    DrawTextEx(font, buffer, (Vector2){0, 0}, TEXT_FONT_SIZE, 0, WHITE);
}

void RenderScene(const Lander* l, const Assets* assets) {
    DrawBackground(assets->background);
    DrawFuelDisplay(assets->font, l->fuel);
    DrawLander(l, assets->sprites);

    if (IsGameOver(l->status))
        DrawEndScreen(assets->font, "Game\nOver", GetGameOverMessage(l->status));

    if (IsSuccess(l->status))
        DrawEndScreen(assets->font, "Success!", "You landed safely!");
}
