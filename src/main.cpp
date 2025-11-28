#include "raylib.h"
#include "raymath.h"

int main()
{
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(0, 0, "YES");

    while(!WindowShouldClose())
    {
        if(IsKeyPressed(KEY_ESCAPE)) break;
        
        BeginDrawing();
        DrawText("Hello", GetScreenWidth()/2 - MeasureText("Hello", 100)/2, GetScreenHeight()/2 - 100 * sin(GetTime()), 100, WHITE);
        SwapScreenBuffer();
        ClearBackground(BLUE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}