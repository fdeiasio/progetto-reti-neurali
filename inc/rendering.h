#ifndef RENDERING_H
#define RENDERING_H

#include "raylib.h"
#include "core.h"

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

void LoadAssets(Assets* assets);
void UnloadAssets(Assets* assets);

// =============== RENDERING ==================

void RenderScene(const Lander* l, const Assets* assets);

#endif //RENDERING_H
