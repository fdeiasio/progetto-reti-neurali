#include <stdio.h>
#include <math.h>

#include "raylib.h"
#include "raymath.h"

// ========================= CONSTANTS =========================

#define WINDOW_HEIGHT   1080
#define WINDOW_WIDTH    1920
#define WINDOW_TITLE    "Lunar Lander"

#define MAX_FPS         60
#define GROUND_HEIGHT   138

#define LANDER_HEIGHT   50
#define LANDER_WIDTH    LANDER_HEIGHT
#define LANDER_INIT_X   300
#define LANDER_INIT_Y   300
#define LANDER_INIT_Z   0

#define STARTING_FUEL   800

#define GRAVITY         120
#define THRUST          200
#define TORQUE          1

#define WIN_START       1140
#define WIN_END         1285
#define VEL_THRESHOLD   100
#define ANGLE_THRESHOLD 15

// ======================= INPUT HANDLING ======================

typedef unsigned char InputMask;
#define INPUT_LEFT      (1 << 0)
#define INPUT_RIGHT     (1 << 1)
#define INPUT_THRUST    (1 << 2)

static InputMask GetInputMask(void) {
    InputMask input = 0;

    if (IsKeyDown(KEY_LEFT))
        input |= INPUT_LEFT;
    if (IsKeyDown(KEY_RIGHT))
        input |= INPUT_RIGHT;
    if (IsKeyDown(KEY_SPACE))
        input |= INPUT_THRUST;

    return input;
}

// ======================== LANDER ==========================

typedef enum {
    IDLE,
    ACTIVE,
    TOO_FAST,
    TOO_ANGLED,
    TOO_FAR,
    OUT_OF_BOUNDS,
    OUT_OF_FUEL,
    WON,
} Status;

typedef struct {
    Vector3 position;  
    Vector3 velocity;  
    int     fuel;
    Status  status;
} Lander;

static Lander lander = {
    .position = {LANDER_INIT_X, LANDER_INIT_Y, LANDER_INIT_Z},
    .velocity = {0, 0, 0},
    .fuel = STARTING_FUEL,
    .status = IDLE,
};

static inline Rectangle GetLanderHitbox(const Lander* l) {
    return (Rectangle) {
        .x = l->position.x - LANDER_WIDTH / 2,
        .y = l->position.y - LANDER_HEIGHT / 2,
        .width = LANDER_WIDTH,
        .height = LANDER_HEIGHT,
    };
}

static void ResetLander(Lander* l) {
    *l = (Lander) {
        .position = {LANDER_INIT_X, LANDER_INIT_Y, LANDER_INIT_Z},
        .velocity = {0, 0, 0},
        .fuel = STARTING_FUEL,
        .status = IDLE,
    };
}

static void ApplyThrust(Lander* l, Vector3* acceleration) {
    l->status = ACTIVE;
    l->fuel--;

    float angle = l->position.z * DEG2RAD;
    acceleration->x += THRUST * sinf(angle);
    acceleration->y -= THRUST * cosf(angle);
}

static void UpdatePosition(Lander* l, float dt, Vector3 acceleration) {
    l->velocity = Vector3Add(l->velocity, Vector3Scale(acceleration, dt));
    l->position = Vector3Add(l->position, Vector3Scale(l->velocity, dt));
}

static bool IsGameOver(const Lander* l) {
    return l->status == TOO_FAST      || 
           l->status == TOO_ANGLED    || 
           l->status == TOO_FAR       || 
           l->status == OUT_OF_BOUNDS || 
           l->status == OUT_OF_FUEL;
}

static void UpdateLander(Lander* l, float dt, InputMask input) {
    if (IsGameOver(l) || l->status == WON)
        return;

    Vector3 acceleration = {0, GRAVITY, 0};

    if (input & INPUT_LEFT)
        l->position.z -= TORQUE;
    if (input & INPUT_RIGHT)
        l->position.z += TORQUE;
    if (input & INPUT_THRUST)
        ApplyThrust(l, &acceleration);
    else
        l->status = IDLE;

    UpdatePosition(l, dt, acceleration);
}

// ====================== SPRITING ========================

typedef enum {
    SPRITE_IDLE = 0,
    SPRITE_THRUST_1 = 1,
    SPRITE_THRUST_2 = 2,
    SPRITE_CRASHED = 3,
    SPRITE_COUNT = 4,
} SpriteIndex;

static void DrawLander(const Lander* l, Texture2D sprite) {
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

static void DrawLanderSprite(const Lander* l, Texture2D sprites[] ,int frame) {
    int sprite_idx;

    if (l->status == IDLE) {
        sprite_idx = SPRITE_IDLE;
    } 
    else if (l->status == ACTIVE) {
        sprite_idx = (frame & (1 << 3)) 
            ? SPRITE_THRUST_1 
            : SPRITE_THRUST_2;
    } 
    else if (l->status == TOO_ANGLED || l->status == TOO_FAR || l->status == TOO_FAST) {
        sprite_idx = SPRITE_CRASHED;
    } 
    else {
        sprite_idx = SPRITE_IDLE;
    }

    DrawLander(l, sprites[sprite_idx]);
}

// ====================== ENVIRONMENT ======================

static Rectangle ground = {
    .x = 0,
    .y = WINDOW_HEIGHT - GROUND_HEIGHT,
    .width = WINDOW_WIDTH,
    .height = GROUND_HEIGHT,
};

static void DrawBackground(Texture2D bg) {
    Rectangle src = {0, 0, bg.width, bg.height};
    Rectangle dst = {0, 0, GetScreenWidth(), GetScreenHeight()};

    DrawTexturePro(bg, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
}

static void CheckBounds(Lander* l) {
    if (l->position.x < 0 || l->position.x > WINDOW_WIDTH ||
        l->position.y < 0 || l->position.y > WINDOW_HEIGHT) {
        l->status = OUT_OF_BOUNDS;
    }
}

static bool IsInWinZone(float x) {
    return x >= WIN_START && x <= WIN_END;
}

static void CheckCollision(Lander* l) {
    if (!CheckCollisionRecs(GetLanderHitbox(l), ground))
        return;

    float speed = Vector2Length((Vector2){l->velocity.x, l->velocity.y});
    
    if (IsInWinZone(l->position.x) && speed < VEL_THRESHOLD && fabsf(l->position.z) < ANGLE_THRESHOLD) {
        l->status = WON;
    } else if (speed >= VEL_THRESHOLD) {
        l->status = TOO_FAST;
    } else if (fabsf(l->position.z) >= ANGLE_THRESHOLD) {
        l->status = TOO_ANGLED;
    } else {
        l->status = TOO_FAR;
    }

    l->position.y = ground.y - LANDER_HEIGHT / 2;
    l->position.z = 0;
    l->velocity = (Vector3){0, 0, 0};
}

static void CheckFuel(Lander* l) {
    if (l->fuel <= 0)
        l->status = OUT_OF_FUEL;
}

static const char* GetGameOverReason(const Lander* l) {
    if (l->status == OUT_OF_FUEL)
        return "The lander has been impacted by oil shortages";
    if (l->status == OUT_OF_BOUNDS)
        return "The lander had ambitions that exceeded its capabilities";
    if (l->status == TOO_FAST)
        return "The lander met the ground too fast for its own good";
    if (l->status == TOO_ANGLED)
        return "The lander tried to land on its side, it failed";
    if (l->status == TOO_FAR)
        return "The lander should calibrate distance better";
    return "Unknown";
}

static void DrawGameOverScreen(Font font, const Lander* l) {
    const char* title = "Game\nOver";
    Vector2 title_size = MeasureTextEx(font, title, 64, 0);
    Vector2 title_pos = {
        WINDOW_WIDTH / 2 - title_size.x / 2,
        WINDOW_HEIGHT / 2 - title_size.y / 2 - 100,
    };
    DrawTextEx(font, title, title_pos, 64, 0, WHITE);

    const char* reason = GetGameOverReason(l);
    Vector2 reason_size = MeasureTextEx(font, reason, 32, 0);
    Vector2 reason_pos = {
        WINDOW_WIDTH / 2 - reason_size.x / 2,
        title_pos.y + title_size.y + 20,
    };
    DrawTextEx(font, reason, reason_pos, 32, 0, WHITE);
}

static void DrawWinScreen(Font font) {
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

static void DrawFuelDisplay(Font font, int fuel) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "Fuel: %d", fuel);
    DrawTextEx(font, buffer, (Vector2){0, 0}, 32, 0, WHITE);
}

// ========================= MAIN ===========================

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
    for (long frame = 0; !WindowShouldClose(); frame++) {
        float dt = GetFrameTime();
        InputMask input = GetInputMask();

        // Reset on game over or win
        if ((IsGameOver(&lander) || lander.status == WON) && IsKeyPressed(KEY_SPACE)) {
            ResetLander(&lander);
            input = 0;
        }

        // Update game state
        UpdateLander(&lander, dt, input);

        CheckBounds(&lander);
        CheckCollision(&lander);
        CheckFuel(&lander);

        // Draw
        BeginDrawing();
            DrawBackground(bg);
            DrawFuelDisplay(font, lander.fuel);
            DrawLanderSprite(&lander, sprites, frame);

            if (IsGameOver(&lander)) {
                DrawGameOverScreen(font, &lander);
            } else if (lander.status == WON) {
                DrawWinScreen(font);
            }
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

    return 0;
}
