#pragma once 
#include <vector>
#include "raylib.h"
#include "sprite.hpp"
#include "raymath.h"

//By design, ALL dice sprites will have 6 images for the rolling animation
class Dice : public Sprite
{
    Vector2 direction;
    float velocity;
    float animSpeed;
    float rotationSpeed;
    int sides;

    bool isRolling;
    Rectangle hitbox;
    Vector2 hitboxSize;

    bool sixFaces; //we got special sprites for 6 faced cube

    float updateTimer;

    int finalNumber;
    
    public:
    Dice(const char* imgPath, Vector2 pos, float scl, Vector2 pixels, Vector2 dir, float velo, float rotSp, int totalSides) //these will be set by (mostly) random values 
        : Sprite(imgPath, pos, scl, pixels)
        //limits for each parameter (to generate):

        //pos = [hitbox.width, hitbox.height <=> GetScreenWidth - hitbox.width, GetScreenHeight - hitbox.height] //random TOO
        ////scl = [1.0f <=> 1.f / num of dices] //NOT random
        ////pixels = always 256 x 256
        //dir = (-)[0.1f, 1.0f] //should use normalization
        //velo = [4.0f , 5.0f] //stops at <=0.3f (0.25f for better accuracy)
            //velo decrements at 0.25f => 15->19 TOTAL ITERATIONS
        //animSp = 0.05f //NOT random, FOR THAT: animSp will NOT be a parameter in the constructor
            //animSp will increment by 0.01f and stop incrementing at 0.15f
        //rotSp =[10.f , 90.f] //no stop condition? stop rotating at <= 10.f
    {
        //isRolling = true;
        isRolling = false;
        direction = dir;
        velocity = velo;
        animSpeed = 0.05f;
        rotationSpeed = rotSp;
        sides = totalSides;
        
        float shrinkFactor = 0.8; //the hitbox will be 80% of the final size
        float hitWidth = pixelSize.x * scale * shrinkFactor;
        float hitHeight = pixelSize.y * scale * shrinkFactor;
        hitboxSize = {hitWidth, hitHeight};
        UpdateHitbox();
        
        sixFaces = (sides == 6 ) ? true : false;

        updateTimer = 0.0f;

        finalNumber = 0.0f;

        SetRow(1);
    }

    void UpdateHitbox()
    {   
        hitbox = 
        {
            position.x - hitboxSize.x/2, position.y - hitboxSize.y/2, hitboxSize.x, hitboxSize.y
        };
    }

    void CheckHitbox() //to be deleted
    {
        DrawRectangleLinesEx(hitbox, 2.5f, RED);
    }

    //to check dice-collisions OUTSIDE (in the main.cpp file):
    Rectangle GetHitbox()
    {
        return hitbox;
    }
    void ChangeDirection(Vector2 multDir) // will take a vector {1 , 1} and (-1) for the axis that gets changed
    {
        direction = Vector2Multiply(direction, multDir);
    }
    void AddPosition(Vector2 addPos)
    {
        position = Vector2Add(position, addPos);
    }
    Vector2 GetDirection()
    {
        return direction;
    }
    
    void SetDirection(Vector2 vec)
    {
        direction = vec;
    }

    //possible dice-to-dice collision checking (inside class):
    // void Dice::CheckDiceCollisions(std::vector<Dice>& dices)
    // {
    //     for (Dice& dice : dices)
    //     {
    //         if (&dice == this) continue; // skip self
    //         if(this < &dice) continue; //no double checking

    //         if (CheckCollisionRecs(hitbox, dice.hitbox))
    //         {
    //             float left   = fmaxf(hitbox.x, dice.hitbox.x);
    //             float right  = fminf(hitbox.x + hitbox.width, dice.hitbox.x + dice.hitbox.width);
    //             float top    = fmaxf(hitbox.y, dice.hitbox.y);
    //             float bottom = fminf(hitbox.y + hitbox.height, dice.hitbox.y + dice.hitbox.height);
    //             float overlapX = right - left;
    //             float overlapY = bottom - top;
    //             if(overlapX > overlapY)
    //             {
    //                 direction.y *= -1;
    //                 dice.direction.y *= -1;
    //             }
    //             else 
    //             {
    //                 direction.x *= -1;
    //                 dice.direction.x *= -1;   
    //             }
    //         }
    //     }
    // }

    void CheckWallCollisions()
    {
        //checking collision with the screen
        if(hitbox.x < 0)
        {
            position.x = hitbox.width / 2; //these lines 'teleport' the hitbox back inside the screen to avoid no-clipping
            direction.x = -direction.x;
        }
        if(hitbox.x + hitbox.width > GetScreenWidth())
        {
            position.x = GetScreenWidth() - (hitbox.width / 2);
            direction.x = -direction.x;
        }
        if(hitbox.y < 0)
        {
            position.y = hitbox.height / 2;
            direction.y = -direction.y;
        }
        if(hitbox.y + hitbox.height > GetScreenHeight())
        {
            position.y = GetScreenHeight() - (hitbox.height / 2);              
            direction.y = -direction.y;
        }
    }

    void Move()
    {
        position = Vector2Add(position, direction * velocity);
        UpdateHitbox();
    }

    void Animate()
    {
        AnimateRow(animSpeed, 6); //6 is hardcoded, all dices have 6 frames for the rolling animation
    }

    void UpdateMembers(float speed) //I'll set speed to 0.25f: every quarter of a second , members update
    {
        updateTimer += GetFrameTime();
        if(updateTimer > speed)
        {
            //hardcoded decrementing values
            updateTimer -= 0.25f;
            velocity -= 0.25f;
            //std::cout << velocity << "\n";
            animSpeed = (animSpeed < 0.15f)? animSpeed + 0.01f : animSpeed; // I should base this off velocity
            //std::cout << animSpeed << "\n";
            rotationSpeed = (rotationSpeed >= 10.f)? rotationSpeed - 5.0f : rotationSpeed;
        }
        if((velocity <= 0.3f) && isRolling) //hardcoded moments of stopping
        {
            isRolling = false;
            finalNumber = GetRandomValue(0, sides - 1); //this will be the method of choosing the random value
            SetRow(0);
            //FOR sixFaces: vvvvv
            SetColumn(finalNumber); 
            //FOR everything else:
        }
    }

    void Update() //Update() will take as a parameter the way in which the number will be selected (to put it in UpdateMember(), where the stopping happens)
    {
        if(isRolling)
        {
            UpdateMembers(0.5f);
            Move();
            Animate();
            Rotate(rotationSpeed);
            //CheckHitbox();
            CheckWallCollisions();
        }
    }

    void SetDefaultValues() //for debugging
    {
        isRolling = true;
        velocity = (float)GetRandomValue(4.f, 5.f); //4.0f slowest speed, 5.0f fastest speed (initial)
        animSpeed = 0.05f; //0.05f slowest speed, 0.01f fastest speed (initial)
        rotationSpeed = 120.f;
        SetRow(1);
    }

    void SetRandomValues()
    {

    }
};