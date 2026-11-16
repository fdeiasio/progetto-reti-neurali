#pragma once

#include <cstdint>

// ========================== WINDOW =======================

// The world fills the whole window
inline constexpr int         kWorldWidth  = 1920;
inline constexpr int         kWorldHeight = 1080;
inline constexpr const char* kWindowTitle = "Lunar Lander";
inline constexpr int         kMaxFps      = 60;

// ========================== LANDER =======================

inline constexpr float    kLanderInitX     = 300;
inline constexpr float    kLanderInitY     = 300;
inline constexpr float    kLanderInitAngle = 0;
inline constexpr uint32_t kLanderInitFuel  = 800;

// ========================== PHYSICS ======================

inline constexpr float kDeltaTime = 0.0167f;
inline constexpr float kGravity   = 120;
inline constexpr float kThrust    = 200;
inline constexpr float kTorque    = 1;

// ========================== LANDING ======================

inline constexpr float kGroundHeight   = 918;
inline constexpr float kWinStart       = 1140;
inline constexpr float kWinEnd         = 1285;
inline constexpr float kVelThreshold   = 100;
inline constexpr float kAngleThreshold = 15;

// ========================== RENDERING ====================

inline constexpr float kLanderHeight = 50;
inline constexpr float kLanderWidth  = kLanderHeight;

inline constexpr int kTitleFontSize = 64;
inline constexpr int kTextFontSize  = 32;
