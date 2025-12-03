#include "button.hpp"

Button::Button(const char* imagePath, Vector2 imagePos, float scale)
{
    Image image = LoadImage(imagePath); //kinda like Sprite in SFML
    int originalWidth = image.width;
    int originalHeight = image.height;
    int newWidth = static_cast<int>(originalWidth * scale);
    int newHeight = static_cast<int>(originalHeight * scale);
    ImageResize(&image, newWidth, newHeight); //now we got the right image + size for our texture, we can put it inside our private texture
    texture = LoadTextureFromImage(image);
    UnloadImage(image);

    position = imagePos; //top-left corner
}
Button::~Button()
{
    UnloadTexture(texture);
}
void Button::Draw()
{
    DrawTextureV(texture, position, WHITE);
}
bool Button::isPressed()
{
    Rectangle rect = {position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height)};

    if(CheckCollisionPointRec(GetMousePosition(), rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        return true;
    }
    else return false;

    
}