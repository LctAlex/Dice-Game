#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <iostream>
#include "button.hpp"
#include "animated_button.hpp"

int main()
{
   // SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(1280, 720, "YES");
    SetTargetFPS(60);
    Camera2D camera2d = {};
    camera2d.zoom = 1.0f; //Crucial
    
    Texture2D background = LoadTexture("assets/menu.png");
    SetTextureFilter(background, TEXTURE_FILTER_BILINEAR);

    Button button1("assets/menu.png", {300, 300}, 0.5f);

    AButton button2("assets/leaf.png", (Vector2){static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())}, {256, 256}, 1.0f);
    button2.setOriginLowerRight();

    while(!WindowShouldClose())
    {
        if(IsKeyPressed(KEY_ESCAPE)) break;

        BeginDrawing();
        ClearBackground(BLUE);

        BeginMode2D(camera2d);
        DrawTexturePro(background,
            (Rectangle){0, 0, (float)background.width, (float)background.height},
            (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
            (Vector2){0, 0},
            0.0f, WHITE);

        button1.Draw();

        button2.Update();
        button2.Draw();
        EndMode2D();

        EndDrawing();
    }
    CloseWindow();

    UnloadTexture(background);

    return 0;
}

