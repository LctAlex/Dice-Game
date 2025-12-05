#include "animated_button.hpp"
#include <iostream>

AButton::AButton(const char* imgPath, Vector2 pos, Vector2 pixelSize, float scale)
{
    rowIndex = colIndex = 0;
    texture = LoadTexture(imgPath);
    position = pos;
    imgSize = pixelSize;
    this->scale = scale;
    origin = {0, 0};
    animTimer = 0.1f;
    direction = 1;
}
AButton::~AButton()
{
    UnloadTexture(texture);
}
void AButton::Draw()
{
    DrawTexturePro(texture,
    (Rectangle){colIndex * imgSize.x, rowIndex * imgSize.y, imgSize.x, imgSize.y},
    (Rectangle){position.x, position.y, imgSize.x * scale, imgSize.y * scale},
    origin, 0.0f, WHITE);
}
void AButton::Update() 
{
    if(AButton::isPressed())
    {
        rowIndex = 1;
        colIndex = 1;
        direction = -1;
        animTimer = 0.0f;
    }
    //everything inside the following if() is hardcoded for the leaf
    if(animTimer >= 0.25f) 
    {
        animTimer -= 0.25f;
        colIndex += direction;
        if(rowIndex == 0)
        {
            if(colIndex == 0 || colIndex == 5) direction *= (-1);
        }
        if(rowIndex == 1)
        {
            if(colIndex == 0) direction *= -1;
            if(colIndex == 4) rowIndex = 0;
        }
    }
    animTimer += GetFrameTime();
}
bool AButton::isPressed()
{
    //Rectangle rect = {position.x - origin.x, position.y - origin.y, static_cast<float>(imgSize.x * scale), static_cast<float>(imgSize.y * scale)};
    //hardcoded rect for the leafsheet
    Rectangle rect = {position.x - origin.x + imgSize.x * 0.3f, position.y - origin.y + imgSize.y * 0.3f, static_cast<float>(imgSize.x * scale) * 0.7f, static_cast<float>(imgSize.y * scale) * 0.7f};
    if(CheckCollisionPointRec(GetMousePosition(), rect) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        return true;
    }
    else return false;
}
void AButton::setScale(float newScale)
{
    scale = newScale;
}
void AButton::setOriginMiddle()
{
    origin = {imgSize.x*scale/2, imgSize.y*scale/2};
}
void AButton::setOriginLowerRight()
{
    origin = {imgSize.x*scale, imgSize.y*scale};
}