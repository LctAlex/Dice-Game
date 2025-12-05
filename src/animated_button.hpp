#pragma once 
#include "raylib.h"

class AButton
{
    private:
    int rowIndex, colIndex;
    Texture2D texture;
    Vector2 position;
    Vector2 imgSize;
    float scale;
    Vector2 origin;
    float animTimer;
    int direction;
    public:
    AButton(const char* imgPath, Vector2 pos, Vector2 pixelSize, float scale);
    ~AButton();
    void Draw();
    void Update();
    bool isPressed();
    void setScale(float newScale);
    void setOriginMiddle();
    void setOriginLowerRight();
};
