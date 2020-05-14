#include "DialogText.h"
#include "DialogManager.h"
#include "Textures.h"
#include "Render.h"


DialogText::DialogText(float x, float y, fMPoint& posText1, fMPoint& posText2, UI* parent, bool interactable) :

	UI({ x, y }, parent, UI_TYPE::RESOURCES_COUNTER, { 0, 0, 0, 0 }, interactable, false, nullptr),

	text1(),
	text2(),

	posText1(posText1),
	posText2(posText2),

	texture2(nullptr)
{
}


DialogText::~DialogText()
{
	app->tex->UnLoad(texture);
}



void DialogText::HandleInput()
{
	P2SString* string = nullptr;

	string = app->dialogManager->GetCurrentString1();

	if (string != nullptr)
	{
		if (text1 != *string)
		{
			text1 = *string;

			texture = ChangeTexture(texture, &text1);
		}
	}
	
	string = app->dialogManager->GetCurrentString2();

	if (string != nullptr) 
	{
		

		if (text2 != *string)
		{
			text2 = *string;

			texture2 = ChangeTexture(texture2, &text2);
		}
	}
}


SDL_Texture* DialogText::ChangeTexture(SDL_Texture* tex, P2SString* string)
{
	app->tex->UnLoad(tex);

	tex = app->fonts->Print(string->GetCharArray(), { 255, 255, 255 }, app->fonts->fonts[0]);

	return tex;
}


void DialogText::Draw(float dt)
{
	
	if (texture != nullptr)
	{
		app->render->Blit(texture, position.x + posText1.x, position.y + posText1.y, nullptr, false, false, '\000', 255, 255, 255);
	}
	if (texture2 != nullptr)
	{
		app->render->Blit(texture2, position.x + posText2.x, position.y + posText2.y, nullptr, false, false, '\000', 255, 255, 255);
	}
}



