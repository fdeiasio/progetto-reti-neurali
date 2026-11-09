#ifndef CORE_H
#define CORE_H

#include <stdbool.h>
#include <stdint.h>

#include "raymath.h"
#include "config.h"

// ========================== STATUS =======================

typedef enum {
    STATUS_IDLE,
    STATUS_ACTIVE,
    STATUS_TOO_FAST,
    STATUS_TOO_ANGLED,
    STATUS_TOO_FAR,
    STATUS_OUT_OF_BOUNDS,
    STATUS_OUT_OF_FUEL,
    STATUS_WON,
} Status;

bool IsGameOver(Status s);
bool IsSuccess(Status s);
bool IsFinished(Status s);

// ======================== ACTION =========================

typedef uint8_t ActionMask;
#define ACTION_LEFT     (1 << 0)
#define ACTION_RIGHT    (1 << 1)
#define ACTION_THRUST   (1 << 2)

// ======================== LANDER ==========================

typedef struct {
    Vector3 position;   // x, y, and z = angle in degrees (0 = upright)
    Vector3 velocity;   // z is unused (the angle changes only via torque)
    int32_t fuel;
    Status  status;
} Lander;

void ResetLander(Lander* l);
void UpdateLander(Lander* l, ActionMask action);

#endif //CORE_H
