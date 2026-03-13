#include <stdio.h>
#include "raylib.h"
#include "raymath.h"

// =============================== CONSTANTS ==================================

#define WINDOW_HEIGHT   1080
#define WINDOW_WIDTH    1920
#define WINDOW_TITLE    "Lunar Lander"

#define MAX_FPS         60

#define GROUND_HEIGHT   200

#define LANDER_HEIGHT   100
#define LANDER_WIDTH    LANDER_HEIGHT
#define LANDER_MASS     500
#define LANDER_POS_X    300
#define LANDER_POS_Y    300
#define LANDER_POS_Z    0

#define gravity 500

// ======================== LUNAR LANDER ===============================

struct LunarLander 
{
    float       mass;
    Vector3     position;
    Vector3     velocity;   // Here the z component represent angular velocity
    
    Rectangle   hitbox;
};

static struct LunarLander lunarLander = 
{
    .mass     = LANDER_MASS,
    .position = 
    { 
        .x = LANDER_POS_X, 
        .y = LANDER_POS_Y, 
        .z = LANDER_POS_Z, 
    },
    .velocity = { 0, 0, 0 },
};

static inline Rectangle LanderHitbox(struct LunarLander* const lunarLander)
{
    return (Rectangle)
    {
        lunarLander->position.x - LANDER_WIDTH / 2,
        lunarLander->position.y - LANDER_HEIGHT / 2,
        LANDER_WIDTH,
        LANDER_HEIGHT,
    };
}

void DrawLander(struct LunarLander* const lunarLander, Texture2D sprite) 
{   
    Rectangle src = { 0, 0, sprite.width, sprite.height };
    Rectangle dst = 
    {
        .x = lunarLander->position.x,
        .y = lunarLander->position.y,
        .width = LANDER_WIDTH,
        .height = LANDER_HEIGHT,
    };
    Vector2 origin = { LANDER_WIDTH / 2, LANDER_HEIGHT / 2 };
    float rot = lunarLander->position.z;

    DrawTexturePro (sprite, src, dst, origin, rot, WHITE);
}

void UpdateLanderPosition(struct LunarLander* lunarLander, Vector3 acceleration, float dt) 
{
    lunarLander->velocity 
        = Vector3Add(lunarLander->velocity, Vector3Scale(acceleration, dt));

    lunarLander->position 
        = Vector3Add(lunarLander->position, Vector3Scale(lunarLander->velocity, dt));
}

// =================================== MISC =====================================

static Rectangle ground = 
{ 
    .x = 0, 
    .y = WINDOW_HEIGHT - GROUND_HEIGHT, 
    .width = WINDOW_WIDTH, 
    .height = GROUND_HEIGHT
};

void DrawBackground(Texture2D background)
{
    DrawTexturePro
    (
        background,
        (Rectangle){ 0, 0, background.width, background.height },
        (Rectangle){ 0, 0, GetScreenWidth(), GetScreenHeight() },
        (Vector2){ 0, 0 },
        0.0f,
        WHITE
    );
}

// =========================== MAIN ==================================

int main(void) 
{  
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(MAX_FPS);

    Texture2D background = LoadTexture("media/bg.jpg");
    Texture2D sprite = LoadTexture("media/lander.png");

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        Vector3 acceleration = { 0, 0, 0 };
        acceleration.y += gravity;

        UpdateLanderPosition(&lunarLander, acceleration, dt);

        BeginDrawing();
            DrawBackground(background);
            DrawLander(&lunarLander, sprite);
        EndDrawing();
    }

    UnloadTexture(background);
    UnloadTexture(sprite); 
    CloseWindow();
}

