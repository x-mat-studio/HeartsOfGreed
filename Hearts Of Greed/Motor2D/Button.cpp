#include "Button.h"
#include "Input.h"
#include "Render.h"
#include "UIManager.h"

Button::Button(fMPoint positionValue, UI* father, SDL_Rect rect, bool dragable, SDL_Texture* texture, BUTTON_TAG buttonTag) :

	UI(positionValue, father, UI_TYPE::BUTTON, rect, true, dragable, texture),

	buttonTag(buttonTag),
	clicked(false),
	focusedFirstFrame(true)
{
}


Button::~Button()
{
	buttonTag = BUTTON_TAG::NULL_TAG;
	clicked = false;
}

BUTTON_TAG Button::GetTag()
{
	return buttonTag;
}


void Button::SetTag(BUTTON_TAG tag)
{
	buttonTag = tag;
}


void Button::HandleInput()
{
	if (focused)
	{
		if (focusedFirstFrame == true)
		{
			app->uiManager->ExecuteHoverButton(buttonTag, this);
			focusedFirstFrame = false;
		}

		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_STATE::KEY_DOWN || app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_STATE::KEY_REPEAT)
		{
			clicked = true;
		}


		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_STATE::KEY_UP && clicked == true)
		{
			app->uiManager->ExecuteButton(buttonTag, this);
			clicked = false;
		}
	}

	else
	{
		clicked = false;
		focusedFirstFrame = true;
	}
}


void Button::Draw(float dt)
{
	Uint8 alpha = 255;
	Uint8 r = 255;
	Uint8 g = 255;
	Uint8 b = 255;

 
   if (focused == true)
    {
		if (clicked == false)
		{
			r = 200;
			g = 200;
			b = 200;
			
			alpha = 225;
		}

		else
		{
			r = 100;
			g = 100;
			b = 100;
		}
      
    }

    app->render->Blit(texture, position.x, position.y, &rect, false, false, alpha, r, g, b);

}