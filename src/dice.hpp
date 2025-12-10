#pragma once 
#include <vector>
#include "raylib.h"
#include "sprite.hpp"
#include "raymath.h"

//By design, ALL dice sprites will have 6 images for the rolling animation
class Dice : public Sprite
{
    Vector2 direction; //float?
    float velocity;
    float animSpeed;
    float rotationSpeed;
    int sides;

    bool isRolling;
    Rectangle hitbox;
    Vector2 hitboxSize;

    bool sixFaces; //we got special sprites for 6 faced cube

    float updateTimer;
    
    public:
    Dice(const char* imgPath, Vector2 pos, float scl, Vector2 pixels, Vector2 dir, float velo, float animSp, float rotSp, int totalSides) //these will be set by (mostly) random values 
        : Sprite(imgPath, pos, scl, pixels)
        //limits for each parameter:

        //pos = [0, 0 <=> GetScreenWidth, GetScreenHeight]
        //scl = [1.0f <=> 1.f / 20]
        //pixels = always 256 x 256
        //dir = [0.0f <=> 1.0f]
        //velo = [0.25f <=> 1.0f] //MAX speed = 1 pixel/iteration, MIN speed = 1/4 pixel/iteration
        //animSp = [0.15f <=> 0.35f] //sprite changes each fraction of a second
        //rotSp =[10.f <=> 90.f] //degrees per second
    {
        isRolling = true;
        direction = dir;
        velocity = velo;
        animSpeed = animSp;
        rotationSpeed = rotSp;
        sides = totalSides;
        
        float shrinkFactor = 0.8; //the hitbox will be 80% of the final size
        float hitWidth = pixelSize.x * scale * shrinkFactor;
        float hitHeight = pixelSize.y * scale * shrinkFactor;
        hitboxSize = {hitWidth, hitHeight};
        UpdateHitbox();
        
        sixFaces = (sides == 6 ) ? true : false;

        updateTimer = 0.0f;

        Sprite::SetRow(1);
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

    void CheckCollisions(std::vector<Dice> dices)
    {
        if(dices.size() > 1)
        {

        }
        //if for screen collision check
    }

    void Move()
    {
        position = Vector2Add(position, direction * velocity);
        UpdateHitbox();
    }

    void Animate()
    {
        Sprite::AnimateRow(animSpeed, 6); //6 is hardcoded, all dices have 6 frames for the rolling animation
    }

    void UpdateMembers()
    {
        updateTimer += GetFrameTime();
        if(updateTimer > 0.25f && isRolling) //every quarter of a second , members update
        {
            velocity -= 0.1f;
            animSpeed -= 0.05f;

        }
    }

    void Update()
    {
        
    }
};