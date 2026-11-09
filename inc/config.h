#ifndef CONFIG_H
#define CONFIG_H

// ========================== WINDOW =======================

// The world fills the whole window
#define WORLD_WIDTH     1920
#define WORLD_HEIGHT    1080

#define WINDOW_TITLE    "Lunar Lander"
#define MAX_FPS         60

// ========================== LANDER =======================

#define LANDER_INIT_X       300
#define LANDER_INIT_Y       300
#define LANDER_INIT_Z       0       // initial angle
#define LANDER_INIT_FUEL    800

// ========================== PHYSICS ======================

#define DELTA_TIME      0.0167
#define GRAVITY         120
#define THRUST          200
#define TORQUE          1

// ========================== LANDING ======================

#define GROUND_HEIGHT   918
#define WIN_START       1140
#define WIN_END         1285
#define VEL_THRESHOLD   100
#define ANGLE_THRESHOLD 15

// ========================== RENDERING ====================

#define LANDER_HEIGHT       50
#define LANDER_WIDTH        LANDER_HEIGHT

#define TITLE_FONT_SIZE     64
#define TEXT_FONT_SIZE      32

#endif //CONFIG_H
