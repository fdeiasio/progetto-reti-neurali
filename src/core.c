#include "core.h"

// ================== STATUS ======================

bool IsGameOver(Status s) {
    return s == STATUS_TOO_FAST      ||
           s == STATUS_TOO_ANGLED    ||
           s == STATUS_TOO_FAR       ||
           s == STATUS_OUT_OF_BOUNDS ||
           s == STATUS_OUT_OF_FUEL;
}

bool IsSuccess(Status s) {
    return s == STATUS_WON;
}

bool IsFinished(Status s) {
    return IsGameOver(s) || IsSuccess(s);
}

// ================== LANDER ======================

void ResetLander(Lander* l) {
    *l = (Lander) {
        .position = {LANDER_INIT_X, LANDER_INIT_Y, LANDER_INIT_Z},
        .velocity = {0, 0, 0},
        .fuel = LANDER_INIT_FUEL,
        .status = STATUS_IDLE,
    };
}

static void ApplyThrust(const Lander* l, Vector3* acceleration) {
    float radians = l->position.z * DEG2RAD;
    acceleration->x += THRUST * sinf(radians);
    acceleration->y -= THRUST * cosf(radians);
}

static void UpdatePosition(Lander* l, ActionMask action) {
    Vector3 acceleration = {0, GRAVITY, 0};

    if (action & ACTION_LEFT)
        l->position.z -= TORQUE;
    if (action & ACTION_RIGHT)
        l->position.z += TORQUE;

    // Keep the angle in [-180, 180] so a full turn counts as upright
    l->position.z = remainderf(l->position.z, 360.0f);

    if (action & ACTION_THRUST)
        ApplyThrust(l, &acceleration);

    l->velocity = Vector3Add(l->velocity, Vector3Scale(acceleration, DELTA_TIME));
    l->position = Vector3Add(l->position, Vector3Scale(l->velocity, DELTA_TIME));
}

static void UpdateFuel(Lander* l, ActionMask action) {
    if (action & ACTION_THRUST)
        l->fuel--;
}

static bool IsLanderOutOfBounds(const Lander* l) {
    return l->position.x < 0 || l->position.x > WORLD_WIDTH ||
           l->position.y < 0 || l->position.y > WORLD_HEIGHT;
}

static bool CheckGroundCollision(const Lander* l) {
    return l->position.y > GROUND_HEIGHT;
}

static Status ApplyGroundCollision(Lander* l) {
    float speed = Vector2Length((Vector2){l->velocity.x, l->velocity.y});

    l->position.y = GROUND_HEIGHT;

    // Check for possible reasons for game over
    if (speed >= VEL_THRESHOLD)
        return STATUS_TOO_FAST;
    if (fabsf(l->position.z) >= ANGLE_THRESHOLD)
        return STATUS_TOO_ANGLED;
    if (l->position.x <= WIN_START || l->position.x >= WIN_END)
        return STATUS_TOO_FAR;

    // If none of it applies, the player must have won
    return STATUS_WON;
}

static void UpdateStatus(Lander* l, ActionMask action) {
    // Each new status check overrides the previous one,
    // so they are ordered by increasing priority
    Status newStatus = STATUS_IDLE;

    if (action & ACTION_THRUST)
        newStatus = STATUS_ACTIVE;

    if (l->fuel <= 0)
        newStatus = STATUS_OUT_OF_FUEL;

    if (CheckGroundCollision(l))
        newStatus = ApplyGroundCollision(l);

    if (IsLanderOutOfBounds(l))
        newStatus = STATUS_OUT_OF_BOUNDS;

    l->status = newStatus;
}

void UpdateLander(Lander* l, ActionMask action) {
    if (IsFinished(l->status))
        return;

    UpdatePosition(l, action);
    UpdateFuel(l, action);
    UpdateStatus(l, action);
}
