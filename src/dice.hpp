#pragma once 
#include "raylib.h"
#include "sprite.hpp"

class Dice : public Sprite
{
    bool sixFaces; //we got special sprites for 6 faced cube
    Vector2 direction; //float?
    float animSpeed;
    float rotationSpeed;
    int sides;
    bool isRolling;

    float velocity;
    Rectangle body;
    
    public:
    Dice(const char* imgPath, Vector2 pos, float scl, Vector2 pixels, Vector2 dir, float animSp, float rotSp, float velo, int totalSides) 
        : Sprite(imgPath, pos, scl, pixels)
    {
        isRolling = true;
        direction = dir;
        animSpeed = animSp;
        rotationSpeed = rotSp;
        sides = totalSides;
        sixFaces = (sides == 6 )? true : false;
    }
};