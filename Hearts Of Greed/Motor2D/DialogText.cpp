#include "DialogText.h"
#include "DialogManager.h"
#include "Textures.h"


DialogText::DialogText(float x, float y, UI* parent, bool interactable) :

	UI({ x, y }, parent, UI_TYPE::RESOURCES_COUNTER, { 0, 0, 0, 0 }, interactable, false, nullptr)
{
	ChangeTexture();
}


DialogText::~DialogText()
{
	app->tex->UnLoad(texture);
}



void DialogText::HandleInput()
{
	if (text != *app->dialogManager->GetCurrentString1())
	{
		app->tex->UnLoad(texture);

		ChangeTexture();
	}
}


void DialogText::ChangeTexture()
{
	text = *app->dialogManager->GetCurrentString1();

	texture = app->fonts->Print(text.GetCharArray(), { 255, 255, 255 }, app->fonts->fonts[0]);
}