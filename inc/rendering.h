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

typedef enum {
    SPRITE_IDLE = 0,
    SPRITE_THRUST_1 = 1,
    SPRITE_THRUST_2 = 2,
    SPRITE_CRASHED = 3,
    SPRITE_COUNT = 4,
} SpriteIndex;

extern void DrawLanderSprite(const Lander* l, Texture2D sprites[], int frame);
extern void DrawFuelDisplay(Font font, int32_t fuel);
extern void DrawBackground(Texture2D bg);
extern void DrawGameOverScreen(Font font, Status s);
extern void DrawWinScreen(Font font);


#endif //RENDERING_H