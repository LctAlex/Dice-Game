#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <iostream>
#include "button.hpp"

int main()
{
   // SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(600, 600, "YES");
    SetTargetFPS(60);
    Camera2D camera2d = {};
    camera2d.zoom = 1.0f; //Crucial

    //HideCursor();

    //Texture2D background = LoadTexture("Tobias.png");
    Button button1("Tobias.png", {0.0f, 0.0f}, 0.3f);

    while(!WindowShouldClose())
    {
        if(IsKeyPressed(KEY_ESCAPE)) break;

        BeginDrawing();
        ClearBackground(BLUE);

        BeginMode2D(camera2d);
        if(button1.isPressed())
        {
            std::cout << "Pressed\n";
        }
        button1.Draw();
       // DrawText("Hello", GetScreenWidth()/2 - MeasureText("Hello", 100)/2, GetScreenHeight()/2, 100, WHITE);
        EndMode2D();

        EndDrawing();
    }
    CloseWindow();
    return 0;
}

