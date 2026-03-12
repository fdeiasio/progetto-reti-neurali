#include <stdio.h>
#include "raylib.h"

#define HEIGHT  1080
#define WIDTH   1920
#define TITLE   "Lunar Lander"

#define GROUND_HEIGHT 200

int main(void) 
{  
    InitWindow(WIDTH, HEIGHT, TITLE);
    SetTargetFPS(60);

    Texture2D background = LoadTexture("media/bg.jpg");

    Rectangle ground = { 0, HEIGHT - GROUND_HEIGHT, WIDTH, GROUND_HEIGHT};

    while (!WindowShouldClose())
    {
        BeginDrawing();

        DrawTexturePro(
            background,
            (Rectangle){ 0, 0, background.width, background.height },
            (Rectangle){ 0, 0, GetScreenWidth(), GetScreenHeight() },
            (Vector2){ 0, 0 },
            0.0f,
            WHITE
        );



        EndDrawing();
    }

    UnloadTexture(background);
    CloseWindow();
    return 0;
}
