#include "TestScene.h"
#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Map.h"

ModuleTestScene::ModuleTestScene()
{

}


ModuleTestScene::~ModuleTestScene()
{

}


bool  ModuleTestScene::Awake(pugi::xml_node&)
{
	return true;
}


// Called before the first frame
bool ModuleTestScene::Start()
{
	//app->map->LoadNew("map_prototype.tmx");
	app->map->LoadNew("isometric_grass_and_water.tmx");

	return true;
}


// Called each loop iteration
bool  ModuleTestScene::PreUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_STATE::KEY_REPEAT)
	{
		app->render->currentCamY += 1;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_STATE::KEY_REPEAT)
	{
		app->render->currentCamY -= 1;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_STATE::KEY_REPEAT)
	{
		app->render->currentCamX += 1;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_STATE::KEY_REPEAT)
	{
		app->render->currentCamX -= 1;
	}

	return true;
}


// Called each loop iteration
bool  ModuleTestScene::Update(float dt)
{
	app->map->Draw();
	return true;
}


// Called each loop iteration
bool  ModuleTestScene::PostUpdate()
{
	return true;
}


// Called before quitting
bool  ModuleTestScene::CleanUp()
{
	return true;
}


bool  ModuleTestScene::Load(pugi::xml_node&)
{
	return true;
}


bool  ModuleTestScene::Save(pugi::xml_node&) const
{
	return true;
}
