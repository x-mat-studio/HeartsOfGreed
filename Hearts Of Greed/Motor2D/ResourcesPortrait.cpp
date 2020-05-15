#include "ResourcesPortrait.h"
#include "Player.h"
#include "Textures.h"
#include "Render.h"


ResourcesPortrait::ResourcesPortrait(float x, float y, UI* parent, bool interactable) :

	UI({ x, y }, parent, UI_TYPE::RESOURCES_COUNTER, { 0, 0, 0, 0 }, interactable, false, nullptr),
	texture2(nullptr),
	texture3(nullptr)
{
	ChangeTexture();
	ChangeTexture2();
	ChangeTexture3();
}


ResourcesPortrait::~ResourcesPortrait()
{
	app->tex->UnLoad(texture);
	app->tex->UnLoad(texture2);
	app->tex->UnLoad(texture3);
}


void ResourcesPortrait::HandleInput()
{
	if ((resourcesNumber != app->player->GetResources()))
	{
		app->tex->UnLoad(texture);
		
		ChangeTexture();
	}
	if ((resourcesNumberSKill != app->player->GetResourcesSkill()))
	{
		app->tex->UnLoad(texture2);

		ChangeTexture2();
	}

	if ((resourcesNumberBoost != app->player->GetResourcesBoost()))
	{
		app->tex->UnLoad(texture3);

		ChangeTexture3();
	}

}


void ResourcesPortrait::ChangeTexture()
{
	char resources[10];

	//resources
	resourcesNumber = app->player->GetResources();

	sprintf_s(resources, 10, "%d", resourcesNumber);

	texture = app->fonts->Print(resources, { 255, 255, 255 }, app->fonts->fonts[0]);
}

void ResourcesPortrait::ChangeTexture2()
{
	char resources[10];

	//skill
	resourcesNumberSKill = app->player->GetResourcesSkill();

	sprintf_s(resources, 10, "%d", resourcesNumberSKill);

	texture2 = app->fonts->Print(resources, { 255, 255, 255 }, app->fonts->fonts[0]);

}

void ResourcesPortrait::ChangeTexture3()
{
	char resources[10];

	//boost
	resourcesNumberBoost = app->player->GetResourcesBoost();

	sprintf_s(resources, 10, "%d", resourcesNumberBoost);

	texture3 = app->fonts->Print(resources, { 255, 255, 255 }, app->fonts->fonts[0]);
}

void ResourcesPortrait::Draw(float dt)
{
	if (texture != nullptr)
	{
		app->render->Blit(texture, position.x +1, position.y + 30, nullptr, false, false, '\000', 255, 255, 255);
	}

	if (texture2 != nullptr)
	{
		app->render->Blit(texture2, position.x +1 , position.y , nullptr, false, false, '\000', 255, 255, 255);
	}

	if (texture3 != nullptr)
	{
		app->render->Blit(texture3, position.x+ 33, position.y , nullptr, false, false, '\000', 255, 255, 255);
	}
}
