#pragma once
#include "raylib.h"
#include "sprite.hpp"

class Cube
{
    private:
    Sprite sprite;
    bool rolling;

    public:
    Cube(const char* imgPath, Vector2 pos, float scale, Vector2 pixels)
    :sprite(imgPath, pos, scale, pixels) //sub-class of this class takes the parameters given
    {
        rolling = true;
    }

    void Roll();
    void Stop();

};