#ifndef RENDERING_H
#define RENDERING_H

#include <stdio.h>

#include "core.h"

// ============== CONFIG =======================

#define WINDOW_HEIGHT   MAX_HEIGHT
#define WINDOW_WIDTH    MAX_WIDTH
#define WINDOW_TITLE    "Lunar Lander"

#define MAX_FPS         60

#define LANDER_HEIGHT   50
#define LANDER_WIDTH    LANDER_HEIGHT

// =============== ASSETS ==================

typedef enum {
    SPRITE_IDLE = 0,
    SPRITE_THRUST_1 = 1,
    SPRITE_THRUST_2 = 2,
    SPRITE_CRASHED = 3,
    SPRITE_COUNT = 4,
} SpriteIndex;

typedef struct {
    Texture2D background;
    Texture2D sprites[SPRITE_COUNT];
    Font font;
} Assets;

extern void LoadAssets(Assets* assets);
extern void UnloadAssets(Assets* assets);

// =============== RENDERING ==================

extern void RenderScene(Lander* const l, Assets* assets);

#endif //RENDERING_H