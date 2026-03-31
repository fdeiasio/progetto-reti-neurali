#include "raylib.h"
#include "rendering.h"
#include "core.h"

// ==================== LANDER ==================

static SpriteIndex GetLanderSpriteIndex(const Lander* l) {
    switch (l->status) {
        case IDLE:
            return SPRITE_IDLE;
        case ACTIVE:
            return (GetTime() * 4) - ((int)(GetTime() * 4)) < 0.5
                ? SPRITE_THRUST_1
                : SPRITE_THRUST_2;
        case TOO_ANGLED:
        case TOO_FAR:
        case TOO_FAST:
            return SPRITE_CRASHED;
        default:
            return SPRITE_IDLE;
    }
}

void DrawLander(const Lander* l, Texture2D sprites[]) {
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

// ====================== ENVIRONMENT ======================

void DrawBackground(Texture2D bg) {
    Rectangle src = {0, 0, bg.width, bg.height};
    Rectangle dst = {0, 0, GetScreenWidth(), GetScreenHeight()};

    DrawTexturePro(bg, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
}

static const char* GetGameOverReason(Status s) {
    switch (s) {
        case OUT_OF_FUEL:
            return "The lander has been impacted by oil shortages";
        case OUT_OF_BOUNDS:
            return "The lander had ambitions that exceeded its capabilities";
        case TOO_FAST:
            return "The lander met the ground too fast for its own good";
        case TOO_ANGLED:
            return "The lander tried to land on its side, it failed";
        case TOO_FAR:
            return "The lander should calibrate distance better";
        default:
            return "Unknown";
    }
}

void DrawGameOverScreen(Font font, Status s) {
    const char* title = "Game\nOver";
    Vector2 title_size = MeasureTextEx(font, title, 64, 0);
    Vector2 title_pos = {
        WINDOW_WIDTH / 2 - title_size.x / 2,
        WINDOW_HEIGHT / 2 - title_size.y / 2 - 100,
    };
    DrawTextEx(font, title, title_pos, 64, 0, WHITE);

    const char* reason = GetGameOverReason(s);
    Vector2 reason_size = MeasureTextEx(font, reason, 32, 0);
    Vector2 reason_pos = {
        WINDOW_WIDTH / 2 - reason_size.x / 2,
        title_pos.y + title_size.y + 20,
    };
    DrawTextEx(font, reason, reason_pos, 32, 0, WHITE);
}

void DrawWinScreen(Font font) {
    const char* title = "Success!";
    Vector2 title_size = MeasureTextEx(font, title, 64, 0);
    Vector2 title_pos = {
        WINDOW_WIDTH / 2 - title_size.x / 2,
        WINDOW_HEIGHT / 2 - title_size.y / 2 - 100,
    };
    DrawTextEx(font, title, title_pos, 64, 0, WHITE);

    const char* subtitle = "You landed safely!";
    Vector2 subtitle_size = MeasureTextEx(font, subtitle, 32, 0);
    Vector2 subtitle_pos = {
        WINDOW_WIDTH / 2 - subtitle_size.x / 2,
        title_pos.y + title_size.y + 20,
    };
    DrawTextEx(font, subtitle, subtitle_pos, 32, 0, WHITE);
}

void DrawFuelDisplay(Font font, int32_t fuel) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "Fuel: %d", fuel);
    DrawTextEx(font, buffer, (Vector2){0, 0}, 32, 0, WHITE);
}