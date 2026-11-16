#include <cmath>
#include <string>

#include "rendering.hpp"

namespace {

constexpr std::array<const char*, Renderer::kSpriteCount> kSpritePaths = {
    "assets/sprite_0.png",
    "assets/sprite_1.png",
    "assets/sprite_2.png",
    "assets/sprite_3.png",
};

Renderer::Sprite GetLanderSprite(Status s) {
    switch (s) {
        case Status::Active:
            return std::fmod(GetTime() * 4, 1.0) < 0.5
                ? Renderer::kSpriteThrust1
                : Renderer::kSpriteThrust2;
        case Status::Too_Angled:
        case Status::Too_Far:
        case Status::Too_Fast:
            return Renderer::kSpriteCrashed;
        case Status::Idle:
        case Status::Out_of_Fuel:
        case Status::Out_of_Bounds:
        case Status::Won:
            return Renderer::kSpriteIdle;
    }
    return Renderer::kSpriteIdle;
}

constexpr const char* GetGameOverMessage(Status s) {
    switch (s) {
        case Status::Out_of_Fuel:
            return "The lander has been impacted by oil shortages";
        case Status::Out_of_Bounds:
            return "The lander had ambitions that exceeded its capabilities";
        case Status::Too_Fast:
            return "The lander met the ground too fast for its own good";
        case Status::Too_Angled:
            return "The lander tried to land on its side, it failed";
        case Status::Too_Far:
            return "The lander should calibrate distance better";
        default:
            return "Unknown";
    }
}

void DrawLander(const Lander& l, const Texture2D& sprite) {
    const Rectangle src{0, 0, float(sprite.width), float(sprite.height)};
    const Rectangle dst{l.x, l.y, kLanderWidth, kLanderHeight};
    const Vector2 origin{kLanderWidth / 2, kLanderHeight / 2};

    DrawTexturePro(sprite, src, dst, origin, l.theta, WHITE);
}

void DrawBackground(const Texture2D& bg) {
    const Rectangle src{0, 0, float(bg.width), float(bg.height)};
    const Rectangle dst{0, 0, float(GetScreenWidth()), float(GetScreenHeight())};

    DrawTexturePro(bg, src, dst, Vector2{0, 0}, 0.0f, WHITE);
}

void DrawEndScreen(const Font& font, const char* title, const char* subtitle) {
    const Vector2 title_size = MeasureTextEx(font, title, kTitleFontSize, 0);
    const Vector2 title_pos{
        GetScreenWidth() / 2 - title_size.x / 2,
        GetScreenHeight() / 2 - title_size.y / 2 - 100,
    };
    DrawTextEx(font, title, title_pos, kTitleFontSize, 0, WHITE);

    const Vector2 subtitle_size = MeasureTextEx(font, subtitle, kTextFontSize, 0);
    const Vector2 subtitle_pos{
        GetScreenWidth() / 2 - subtitle_size.x / 2,
        title_pos.y + title_size.y + 20,
    };
    DrawTextEx(font, subtitle, subtitle_pos, kTextFontSize, 0, WHITE);
}

void DrawFuelDisplay(const Font& font, uint32_t fuel) {
    const std::string text = "Fuel: " + std::to_string(fuel);
    DrawTextEx(font, text.c_str(), Vector2{0, 0}, kTextFontSize, 0, WHITE);
}

}  // namespace

Renderer::Renderer() {
    InitWindow(kWorldWidth, kWorldHeight, kWindowTitle);
    SetTargetFPS(kMaxFps);

    background_ = LoadTexture("assets/bg.jpg");
    for (std::size_t i = 0; i < kSpriteCount; ++i)
        sprites_[i] = LoadTexture(kSpritePaths[i]);
    font_ = LoadFontEx("assets/font.ttf", kTextFontSize, nullptr, 250);
}

Renderer::~Renderer() {
    UnloadTexture(background_);
    for (const Texture2D& sprite : sprites_)
        UnloadTexture(sprite);
    UnloadFont(font_);

    CloseWindow();
}

void Renderer::Draw(const Lander& l) const {
    BeginDrawing();

    DrawBackground(background_);
    DrawFuelDisplay(font_, l.fuel);
    DrawLander(l, sprites_[GetLanderSprite(l.status)]);

    if (IsGameOver(l.status))
        DrawEndScreen(font_, "Game\nOver", GetGameOverMessage(l.status));

    if (IsSuccess(l.status))
        DrawEndScreen(font_, "Success!", "You landed safely!");

    EndDrawing();
}
