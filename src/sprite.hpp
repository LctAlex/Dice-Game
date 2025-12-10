#pragma once
#include "raylib.h"
#include <iostream>

class Sprite
{
    protected: //protected so I cam use them other classes
    Texture texture;
    Vector2 pixelSize;
    float passedTime;
    float scale;
    public:
    Vector2 position;
    int rowIndex, colIndex; //colIndex will be used the most
    float rotation;

    Sprite(const char* imgPath, Vector2 pos, float scl, Vector2 pixels)
    {
        texture = LoadTexture(imgPath);

        position = pos;
        pixelSize = pixels;

        rowIndex = 0;
        colIndex = 0;
        rotation = 0.f;
        scale = scl;

        passedTime = 0;
    }
    ~Sprite()
    {
        UnloadTexture(texture);
    }
    void Draw()
    {                      //vvv Column * width vvv  vvv Row * height vvv
        Rectangle source = {colIndex * pixelSize.x, rowIndex * pixelSize.y, pixelSize.x, pixelSize.y}; //start on the spritesheet's x and y ; size of the pixels on spritesheet
        Rectangle dest = {position.x, position.y, pixelSize.x * scale, pixelSize.y * scale}; //position on the screen, final size (obtained inside constructor)

        DrawTexturePro(texture, source, dest, {pixelSize.x * scale/2, pixelSize.y * scale/2}, rotation, WHITE);
    }
    void SetRow(int row)
    {
        rowIndex = row;
    }
    void SetColumn(int col)
    {
        colIndex = col;
    }
    void AnimateRow(float speed, int colNum)
    {
        static float timer = 0.f;
        timer += GetFrameTime();
        if(timer > speed)
        {
            timer -= speed;
            colIndex = (colIndex + 1) % colNum;
        }
    }
    void Rotate(float speed) //will rotate 'speed' degrees per second
    {
        float degrees = speed * GetFrameTime(); //degrees per second

        rotation += degrees;

        if(rotation >= 360.f)rotation -= 360.f;
        if(rotation < 0.f)rotation += 360.f;
    }
};