#include "core.h"
#include "raymath.h"

// ================== STATUS ======================

bool IsGameOver(Status s) {
    return s == TOO_FAST      || 
           s == TOO_ANGLED    || 
           s == TOO_FAR       || 
           s == OUT_OF_BOUNDS || 
           s == OUT_OF_FUEL;
}

bool IsSuccess(Status s) {
    return s == WON;
}

// ================== LANDER ======================

void ResetLander(Lander* l) {
    *l = (Lander) {
        .position = {LANDER_INIT_X, LANDER_INIT_Y, LANDER_INIT_Z},
        .velocity = {0, 0, 0},
        .fuel = LANDER_INIT_FUEL,
        .status = IDLE,
    };
}

static void ApplyThrust(Lander* const l, Vector3* acceleration) {
    float angle = l->position.z * DEG2RAD;
    acceleration->x += THRUST * sinf(angle);
    acceleration->y -= THRUST * cosf(angle);
}

static void UpdatePosition(Lander* l, ActionMask action) {
    Vector3 acceleration = {0, GRAVITY, 0};

    if (action & ACTION_LEFT)
        l->position.z -= TORQUE;
    if (action & ACTION_RIGHT)
        l->position.z += TORQUE;
    if (action & ACTION_THRUST)
        ApplyThrust(l, &acceleration);

    l->velocity = Vector3Add(l->velocity, Vector3Scale(acceleration, DELTA_TIME));
    l->position = Vector3Add(l->position, Vector3Scale(l->velocity, DELTA_TIME));
}

static void UpdateFuel(Lander* l, ActionMask action) {
    if (action & ACTION_THRUST)
        l->fuel--;
}

static bool IsLanderOutOfBounds(Lander* const l) {
    return l->position.x < 0 || l->position.x > MAX_WIDTH ||
           l->position.y < 0 || l->position.y > MAX_HEIGHT;
}

static bool CheckGroundCollision(Lander* const l) {
    return l->position.y > GROUND_HEIGHT;
}

static Status ApplyGroundCollision(Lander* const l) {
    l->position.y = GROUND_HEIGHT;
    l->position.z = 0;
    
    float speed = Vector2Length((Vector2){l->velocity.x, l->velocity.y});
    
    if (speed >= VEL_THRESHOLD)
        return TOO_FAST;

    if (fabsf(l->position.z) >= ANGLE_THRESHOLD)
        return TOO_ANGLED;

    if (l->position.x <= WIN_START && l->position.x >= WIN_END)
        return TOO_FAR;

    return WON;
}

static void UpdateStatus(Lander* l, ActionMask action) {
    Status newStatus = IDLE;

    if (action & ACTION_THRUST)
        newStatus = ACTIVE;

    if (CheckGroundCollision(l)) 
        newStatus = ApplyGroundCollision(l);

    if (l->fuel <= 0)
        newStatus = OUT_OF_FUEL;

    if (IsLanderOutOfBounds(l))
        newStatus = OUT_OF_BOUNDS;

    l->status = newStatus;
}

void UpdateLander(Lander* l, ActionMask action) {
    if (IsGameOver(l->status) || IsSuccess(l->status))
        return;

    UpdatePosition(l, action);
    UpdateFuel(l, action);
    UpdateStatus(l, action);
}
