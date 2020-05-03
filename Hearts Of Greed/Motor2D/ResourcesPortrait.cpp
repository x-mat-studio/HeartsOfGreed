#include "ResourcesPortrait.h"
#include "Player.h"
#include "Textures.h"


ResourcesPortrait::ResourcesPortrait(float x, float y, UI* parent, bool interactable) :

	UI({ x, y }, parent, UI_TYPE::RESOURCES_COUNTER, { 0, 0, 0, 0 }, interactable, false, nullptr)
{
	ChangeTexture();
}


ResourcesPortrait::~ResourcesPortrait()
{
}


void ResourcesPortrait::HandleInput()
{
	if (resourcesNumber != app->player->GetResources())
	{
		app->tex->UnLoad(texture);
		
		ChangeTexture();
	}

}


void ResourcesPortrait::ChangeTexture()
{
	char resources[10];

	resourcesNumber = app->player->GetResources();

	sprintf_s(resources, 10, "%d", resourcesNumber);

	texture = app->fonts->Print(resources, { 255, 255, 255 }, app->fonts->fonts[0]);

}