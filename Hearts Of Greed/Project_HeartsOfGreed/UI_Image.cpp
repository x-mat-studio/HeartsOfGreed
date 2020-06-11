#include "UI_Image.h"

UI_Image::UI_Image(float x, float y, UI* parent, SDL_Rect rect, SDL_Texture* texture, bool dragable, bool interactable) :
    
    UI({ x, y }, parent, UI_TYPE::IMG, rect, interactable, dragable, texture)
{}

UI_Image::~UI_Image()
{}

