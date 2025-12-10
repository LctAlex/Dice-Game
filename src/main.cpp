#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <iostream>
#include "button.hpp"
#include "animated_button.hpp"
#include "dice.hpp"

#include "sprite.hpp"

int main()
{
   // SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(1280, 720, "YES");
    SetTargetFPS(60);
    Camera2D camera2d = {};
    camera2d.zoom = 1.0f; //Crucial
    
    Texture2D background = LoadTexture("assets/menu.png");
    //SetTextureFilter(background, TEXTURE_FILTER_BILINEAR);

    AButton button2("assets/leaf.png", (Vector2){static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())}, {256, 256}, 1.0f);
    button2.setOriginLowerRight();

    Sprite sprite("assets/dice6.png", {(float)GetScreenWidth()/2, (float)GetScreenHeight()/2}, .5f, {256, 256});
    sprite.SetRow(1);

    Dice dice("assets/dice6.png", {(float)GetScreenWidth()/2, (float)GetScreenHeight()/2}, 1.0f, {256, 256}, {0.1f, 0.1f}, 0.15f, 0.15f, 0.5f, 6);
    dice.SetRow(1);

    int chosenVal = -1;
    float timer = 0.0f;
    while(!WindowShouldClose())
    {
        if(IsKeyPressed(KEY_ESCAPE)) break;
        if(IsKeyPressed(KEY_ENTER)) {chosenVal = -1; timer = 0.0f; sprite.SetRow(1); dice.SetRow(1);}

        BeginDrawing();
        ClearBackground(WHITE);

        BeginMode2D(camera2d);
        DrawTexturePro(background,
            (Rectangle){0, 0, (float)background.width, (float)background.height},
            (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
            (Vector2){0, 0},
            0.0f, WHITE);

        button2.Update();
        button2.Draw();
        //DrawText("Bia", 300, 300, 20, RED);
        if(chosenVal == -1)
        {
            timer += GetFrameTime();

            if(timer > 2.f)
            {
                // sprite.SetRow(0);
                // sprite.SetColumn(chosenVal);
                // std::cout << chosenVal;
                dice.SetRow(0);
                chosenVal = GetRandomValue(0, 5);
                dice.SetColumn(chosenVal);
                std::cout << chosenVal;
            }
            else
            {
                // sprite.AnimateRow(0.05f, 6);
                // sprite.Rotate(0.15f, 15);
                // sprite.AnimateRow(0.05f, 6);
                dice.Rotate(0.15f, 15);
                dice.AnimateRow(0.05f, 6);
            }
        }
        sprite.Draw();
        dice.Draw();

        // DrawLine(0, GetScreenHeight()/2, GetScreenWidth(), GetScreenHeight()/2, BLUE);
        // DrawLine(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenHeight(), RED);

        EndMode2D();

        EndDrawing();
    }
    CloseWindow();

    UnloadTexture(background);

    return 0;
}

