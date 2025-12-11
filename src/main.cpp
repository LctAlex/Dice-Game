#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <iostream>
#include <vector>

#include "button.hpp"
#include "animated_button.hpp"
#include "dice.hpp"

#include "sprite.hpp"

enum Sides{D6, D8, D10, D12, D20};
enum Amounts{OneD, TwoD, ThreeD, FiveD, TenD, FifteenD, TwentyD};
enum Menus{/*fadeIn,*/ MainMenu, Options, Info, Game, Results};

//functions I should put in separate files:

// std::vector<Dice> GenerateDices(Sides sides, Amounts amounts)
// {
//     std::vector<Dice> dices;
//     int diceSides = 0;
//     int diceAmounts = 0;
//     const char* imgPath = "";
//     switch(sides)
//     {
//         case D6: diceSides = 6; imgPath = "assets/dice6.png"; break;
//         case D8: diceSides = 8; imgPath = "assets/dice8.png"; break;
//         case D10: diceSides = 10; imgPath = "assets/dice10.png"; break;
//         case D12: diceSides = 12; imgPath = "assets/dice12.png";break;
//         case D20: diceSides = 20; imgPath = "assets/dice20.png";break;
//         default: diceSides = 6; imgPath = "assets/dice6.png"; break;
//     }

//     switch(amounts)
//     {
//         case OneD: diceAmounts = 1;
//         case TwoD: diceAmounts = 2;
//         case ThreeD: diceAmounts = 3;
//         case FiveD: diceAmounts = 5;
//         case TenD: diceAmounts = 10;
//         case FifteenD: diceAmounts = 15;
//         case TwentyD: diceAmounts = 20;
//         default: diceAmounts = 1;
//     }

//     // for(int i = 0; i < diceAmounts; i++)
//     // {
//     //     dices[i] = Dice(imgPath,...,{256, 256}, HomemadeFunction(min, max) <- that takes random float values in intervals(included))
//     // }
// }

//GENERATE SEPARATELY
// for each new dice:
//     pos = random
//     for each other dice:
//         if collision -> pos.x += some amount

//will delete:
float GetSign (float number)
{
    return (number == 0) ? 0 : number / fabs(number); //get the sign of a number
}
Vector2 RandomDirection() //I will do this one myself and I'll normalize myself
{
    Vector2 dir = {
        (float)GetRandomValue(-100, 100),
        (float)GetRandomValue(-100, 100)
    };
    float len = Vector2Length(dir);
    if (len < 0.1f) return {1.0f, 0.1f}; // safety
    return Vector2Scale(dir, 1.0f / len); // normalized
}

void CheckDiceCollisions(std::vector<Dice>& dices) // this comes after ALL individual updates (after for(auto& dice: dices){dice.Update()})
{
    for(int i = 0; i < (int)dices.size(); i++)
    {
        for(int j = i + 1; j < (int)dices.size(); j++) //j = i + 1 ensures NO DUPLICATE calculations
        {
            Dice& A = dices[i]; //simply a rename for dice[i]
            Dice& B = dices[j];
            float aIsColliding = false;
            float bIsColliding = false;

            if(CheckCollisionRecs(A.GetHitbox(), B.GetHitbox()))
            {
                float left = fmaxf(A.GetHitbox().x, B.GetHitbox().x);
                float right = fminf(A.GetHitbox().x + A.GetHitbox().width, B.GetHitbox().x + B.GetHitbox().width);
                float top = fmaxf(A.GetHitbox().y, B.GetHitbox().y);
                float bottom = fminf(A.GetHitbox(). y + A.GetHitbox().height, B.GetHitbox(). y + B.GetHitbox().height);

                float overlapX = right - left;
                float overlapY = bottom - top;
                //we'll use elasticity (swap direction values between dices)
                if(overlapX > overlapY) 
                {
                    int offset = overlapY * 2;
                    float aDirY = A.GetDirection().y;
                    float bDirY = B.GetDirection().y;
                    if(!aIsColliding) A.SetDirection({A.GetDirection().x, bDirY});
                    A.AddPosition({0,GetSign(bDirY) * offset});
                    if(!bIsColliding) B.SetDirection({B.GetDirection().x, aDirY});
                    B.AddPosition({0,GetSign(aDirY) * offset});

                    // A.ChangeDirection({1, -1});
                    // A.AddPosition({0,A.GetDirection().y/abs(A.GetDirection().y) * offset});
                    // B.ChangeDirection({1, -1});
                    // B.AddPosition({0, B.GetDirection().y/abs(B.GetDirection().y) * offset});

                    // A.ChangeDirection({1, -1});
                    // A.AddPosition({0,A.GetDirection().y * offset});
                    // B.ChangeDirection({1, -1});
                    // B.AddPosition({0, B.GetDirection().y * offset});             
                }
                else 
                {
                    int offset = overlapX * 2;
                    float aDirX = A.GetDirection().x;
                    float bDirX = B.GetDirection().x;
                    if(!aIsColliding) A.SetDirection({bDirX, A.GetDirection().y});
                    B.AddPosition({GetSign(aDirX) * offset, 0});
                    if(!bIsColliding) B.SetDirection({aDirX, B.GetDirection().y});
                    A.AddPosition({GetSign(bDirX) * offset, 0}); 
                    
                    // A.ChangeDirection({-1, 1});
                    // A.AddPosition({A.GetDirection().x/fabs(A.GetDirection().x) * offset, 0});
                    // B.ChangeDirection({-1, 1});
                    // B.AddPosition({B.GetDirection().x/fabs(B.GetDirection().x) * offset, 0});

                    // A.ChangeDirection({-1, 1});
                    // A.AddPosition({GetSign(B.GetDirection().x) * offset, 0});
                    // B.ChangeDirection({-1, 1});
                    // B.AddPosition({GetSign(A.GetDirection().x) * offset, 0});
                }
            }
        }
    }
}

int main()
{
    InitWindow(1280, 720, "YES");
    SetTargetFPS(60);
    Camera2D camera2d = {};
    camera2d.zoom = 1.0f; //Crucial
    
    Texture2D background = LoadTexture("assets/menu.png");

    //Animated button will be removed, I'm not using this anyway
    AButton button2("assets/leaf.png", (Vector2){static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())}, {256, 256}, 1.0f);
    button2.setOriginLowerRight();

    Dice dice("assets/dice6.png", /*{0, 0}*/{(float)GetScreenWidth()/2, (float)GetScreenHeight()/2}, 0.25f, {256, 256},
                                     {1.0f, 1.0f}, 1.0f, 90.f, 6);

    std::vector<Dice> dices;
    dices.reserve(20); //max dices: 20. We won't need to reallocate later
    for(int i = 0; i < 20; i++)
    {
        //all will be random
        Vector2 position = {(float)GetScreenWidth()/2, (float)GetScreenHeight()/2};
        float scale = 0.25f;
        Vector2 direction = RandomDirection();
        float velocity = 10.0f;
        float rotation = 90.0f;
        int sides = 6; //not random
        const char* imgPath = "assets/dice6.png"; //not random
        dices.emplace_back(imgPath, position, scale, (Vector2){256, 256}, direction, velocity, rotation, sides);
    }

    //to clear the vector:
    //dices.clear();

    while(!WindowShouldClose())
    {
        if(IsKeyPressed(KEY_ESCAPE)) break;

        if(IsKeyPressed(KEY_ENTER))
        {
            for(auto& dice: dices)
            {
                dice.SetDefaultValues();
            }
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

        // dice.Update();
        // dice.Draw();

        for(auto& dice: dices)
        {
            dice.Update();
            dice.Draw();
        }
        if(dices.size() <= 20) CheckDiceCollisions(dices);

        EndMode2D();

        EndDrawing();
    }
    CloseWindow();

    UnloadTexture(background);

    return 0;
}

