#ifndef CORE_H
#define CORE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "raymath.h"

// ========================== CONFIG =======================

#define LANDER_INIT_X       300
#define LANDER_INIT_Y       300
#define LANDER_INIT_Z       0
#define LANDER_INIT_FUEL    800


#define DELTA_TIME      0.0167         
#define GRAVITY         120
#define THRUST          200
#define TORQUE          1


#define MAX_WIDTH       1920
#define MAX_HEIGHT      1080


#define GROUND_HEIGHT   918
#define WIN_START       1140
#define WIN_END         1285
#define VEL_THRESHOLD   100
#define ANGLE_THRESHOLD 15

// ========================== STATUS =======================

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

extern bool IsGameOver(Status s);
extern bool IsSuccess(Status s);

// ======================== ACTION =========================

typedef uint8_t ActionMask;
#define ACTION_LEFT     (1 << 0)
#define ACTION_RIGHT    (1 << 1)
#define ACTION_THRUST   (1 << 2)

// ======================== LANDER ==========================

typedef struct {
    Vector3 position;  
    Vector3 velocity;  
    int32_t fuel;
    Status  status;
} Lander;

extern void ResetLander(Lander* l);
extern void UpdateLander(Lander* l, ActionMask action);


#endif //CORE_H