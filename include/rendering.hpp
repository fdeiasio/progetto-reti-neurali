#pragma once

#include <array>
#include <cstdint>

#include "raylib.h"
#include "core.hpp"

// Owns the window and the assets: raylib needs the window open before loading textures
class Renderer {
public:
    enum Sprite : std::uint8_t {
        kSpriteIdle,
        kSpriteThrust1,
        kSpriteThrust2,
        kSpriteCrashed,
        kSpriteCount,
    };

    Renderer();
    ~Renderer();
    Renderer(const Renderer&)            = delete;
    Renderer& operator=(const Renderer&) = delete;

    [[nodiscard]] bool ShouldClose() const;
    void Draw(const Lander& lander) const;

private:
    Texture2D                            background_{};
    std::array<Texture2D, kSpriteCount>  sprites_{};
    Font                                 font_{};
};
