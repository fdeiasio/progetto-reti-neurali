#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include "raylib.h"
#include "raymath.h"

// =============================== CONSTANTS ==================================

#define WINDOW_HEIGHT   1080
#define WINDOW_WIDTH    1920
#define WINDOW_TITLE    "Lunar Lander"

#define MAX_FPS         60

#define GROUND_HEIGHT   138     // Hard-coded, don't change

#define LANDER_HEIGHT   50
#define LANDER_WIDTH    LANDER_HEIGHT
#define LANDER_POS_X    300
#define LANDER_POS_Y    300
#define LANDER_POS_Z    0
#define STARTING_FUEL   1000

#define gravity 120
#define thrust  200
#define torque  1

// ======================== LUNAR LANDER ===============================

enum landerStatus  {
    IDLE,
    ACTIVE,
    CRASHED,
    OOB,        // Out Of Bounds
    OOF,        // Out Of Fuel
    STATES_NUM,
};
 
struct LunarLander {
    Vector3 position;
    Vector3 velocity;   // Here the z component represent angular velocity
    int     fuel;

    enum landerStatus status;
};

static struct LunarLander lunarLander = {
    .position = { 
        .x = LANDER_POS_X, 
        .y = LANDER_POS_Y, 
        .z = LANDER_POS_Z, 
    },
    .velocity = { 0, 0, 0 },
    .fuel     = STARTING_FUEL,
    .status   = IDLE,
};

static inline Rectangle LanderHitbox(struct LunarLander* const lunarLander) {
    return (Rectangle) {
        lunarLander->position.x - LANDER_WIDTH / 2,
        lunarLander->position.y - LANDER_HEIGHT / 2,
        LANDER_WIDTH,
        LANDER_HEIGHT,
    };
}

void ResetLander(struct LunarLander* lunarLander) {
    *lunarLander = (struct LunarLander) {
        .position = { 
            .x = LANDER_POS_X, 
            .y = LANDER_POS_Y, 
            .z = LANDER_POS_Z, 
        },
        .velocity = { 0, 0, 0 },
        .fuel     = STARTING_FUEL,
        .status   = IDLE,
    };
}

void DrawLander(struct LunarLander* const lunarLander, Texture2D sprite) {   
    Rectangle src = { 0, 0, sprite.width, sprite.height };
    Rectangle dst = {
        .x = lunarLander->position.x,
        .y = lunarLander->position.y,
        .width = LANDER_WIDTH,
        .height = LANDER_HEIGHT,
    };
    Vector2 origin = { LANDER_WIDTH / 2, LANDER_HEIGHT / 2 };
    float rot = lunarLander->position.z;

    DrawTexturePro (sprite, src, dst, origin, rot, WHITE);
}

void UpdateLanderPosition(struct LunarLander* lunarLander, float dt) {
    if (lunarLander->status == CRASHED || 
        lunarLander->status == OOB  ||
        lunarLander-> status == OOF
    ) 
        return;

    Vector3 acceleration = { 0, 0, 0 };
    acceleration.y += gravity;

    if (IsKeyDown(KEY_LEFT))
        lunarLander->position.z -= torque;

    if (IsKeyDown(KEY_RIGHT))
        lunarLander->position.z += torque;

    if (IsKeyDown(KEY_SPACE)) {
        lunarLander->status = ACTIVE;

        lunarLander->fuel--;
        printf("%d\n", lunarLander->fuel);

        acceleration.x += thrust * sinf(lunarLander->position.z * DEG2RAD);
        acceleration.y -= thrust * cosf(lunarLander->position.z * DEG2RAD);
    } else {
        lunarLander->status = IDLE;
    }

    lunarLander->velocity 
        = Vector3Add(lunarLander->velocity, Vector3Scale(acceleration, dt));

    lunarLander->position 
        = Vector3Add(lunarLander->position, Vector3Scale(lunarLander->velocity, dt));
}

// =================================== SCENE =====================================

static Rectangle ground = { 
    .x = 0, 
    .y = WINDOW_HEIGHT - GROUND_HEIGHT, 
    .width = WINDOW_WIDTH, 
    .height = GROUND_HEIGHT
};

void DrawBackground(Texture2D background){
    DrawTexturePro (
        background,
        (Rectangle){ 0, 0, background.width, background.height },
        (Rectangle){ 0, 0, GetScreenWidth(), GetScreenHeight() },
        (Vector2){ 0, 0 },
        0.0f,
        WHITE
    );
}

// =========================== MAIN ==================================

char buffer[1024];

int main(void) {  
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

    SetTargetFPS(MAX_FPS);

    Texture2D background = LoadTexture("media/bg.jpg");
    Texture2D sprite_0 = LoadTexture("media/sprite_0.png");
    Texture2D sprite_1 = LoadTexture("media/sprite_1.png");
    Texture2D sprite_2 = LoadTexture("media/sprite_2.png");
    Texture2D sprite_3 = LoadTexture("media/sprite_3.png");

    Font font = LoadFontEx("media/font.ttf", 32, 0, 250);

    for (long frame = 0; !WindowShouldClose(); frame++) {
        float dt = GetFrameTime();
        
        UpdateLanderPosition(&lunarLander, dt);

        if (lunarLander.position.x < 0 ||
            lunarLander.position.x > WINDOW_WIDTH ||
            lunarLander.position.y < 0 ||
            lunarLander.position.y > WINDOW_HEIGHT
        ) {
            lunarLander.status = OOB;
        }

        if(CheckCollisionRecs(LanderHitbox(&lunarLander), ground)) {
            lunarLander.status = CRASHED;
            lunarLander.position.y = ground.y - LANDER_HEIGHT / 2;
            lunarLander.position.z = 0;
            lunarLander.velocity = (Vector3) { 0, 0, 0 };
        }

        if (lunarLander.fuel <= 0) {
            lunarLander.status = OOF;
        }

        BeginDrawing();
        DrawBackground(background);

        sprintf(buffer, "Fuel: %d", lunarLander.fuel);
        DrawTextEx(font, buffer, (Vector2){0, 0}, 32, 0, WHITE);

        switch (lunarLander.status) {
            case (IDLE):
                DrawLander(&lunarLander, sprite_0);
                break;
            case (ACTIVE):
                if (frame & (1 << 3))
                    DrawLander(&lunarLander, sprite_1);
                else
                    DrawLander(&lunarLander, sprite_2);
                break;
            case (CRASHED):
                DrawLander(&lunarLander, sprite_3);
                break;
            default:
                break;
        }

        if (lunarLander.status == CRASHED) {
            DrawTextPro(
                font, 
                "Game\nOver", 
                (Vector2){WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2},
                64, 
                0,
                WHITE
            );
        }


        EndDrawing();
    }

    UnloadTexture(background);
    UnloadTexture(sprite_0);
    UnloadTexture(sprite_1);
    UnloadTexture(sprite_2);
    UnloadTexture(sprite_3);

    CloseWindow();
}

