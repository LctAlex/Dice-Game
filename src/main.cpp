#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <iostream>
#include "button.hpp"
#include "animated_button.hpp"
#include "dice.hpp"

#include "sprite.hpp"

enum Sides{D6, D8, D10, D12, D20};
enum Amounts{OneD, TwoD, ThreeD, FiveD, TenD, FifteenD, TwentyD};
enum Menus{/*fadeIn,*/ MainMenu, Options, Info, Game, Results};

// void GenerateDice(Sides sides, Amounts amounts)
// {

// }

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

    Dice dice("assets/dice6.png", {(float)GetScreenWidth()/2, (float)GetScreenHeight()/2}, 0.25f, {256, 256}, {1.0f, 1.0f}, 0.5f, 0.15f, 0.15f, 6);
    dice.SetRow(1);

    float rotationSpeed = 0.0f;
    while(!WindowShouldClose())
    {
        if(IsKeyPressed(KEY_ESCAPE)) break;

        if(IsKeyPressed(KEY_LEFT))
        {
            rotationSpeed -= 5.f;
            std::cout << rotationSpeed << "\n";
        }
        if(IsKeyPressed(KEY_RIGHT))
        {
            rotationSpeed += 5.f;
            std::cout << rotationSpeed << "\n";
        }

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

        dice.Rotate(rotationSpeed);
        dice.Move();
        dice.Draw();

        EndMode2D();

        EndDrawing();
    }
    CloseWindow();

    UnloadTexture(background);

    return 0;
}

