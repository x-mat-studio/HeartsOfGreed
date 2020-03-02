#include "TestScene.h"
#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"

ModuleTestScene::ModuleTestScene() :prevMousePosX(0), prevmousePosY(0)
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
	app->map->LoadNew("map_prototype.tmx");

	return true;
}


// Called each loop iteration
bool  ModuleTestScene::PreUpdate()
{
	return true;
}


// Called each loop iteration
bool  ModuleTestScene::Update(float dt)
{
	float scale = app->win->scale;
	float camVel = 10 * scale;


	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_STATE::KEY_REPEAT)
	{
		camVel *= 2;
	}
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_STATE::KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_UP) == KEY_STATE::KEY_REPEAT)
	{
		app->render->currentCamY += camVel * dt;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_STATE::KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_STATE::KEY_REPEAT)
	{
		app->render->currentCamY -= camVel * dt;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_STATE::KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_STATE::KEY_REPEAT)
	{
		app->render->currentCamX += camVel * dt;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_STATE::KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_STATE::KEY_REPEAT)
	{
		app->render->currentCamX -= camVel * dt;
	}

	if (app->input->GetMouseButtonDown(1) == KEY_STATE::KEY_DOWN || app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_STATE::KEY_DOWN)
	{
		app->input->GetMousePosition(prevMousePosX, prevmousePosY);
	}
	else if (app->input->GetMouseButtonDown(1) == KEY_STATE::KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_STATE::KEY_REPEAT)
	{
		int auxX = 0;
		int auxY = 0;
		int x = 0;
		int y = 0;
		app->input->GetMousePosition(x, y);
		auxX = x;
		auxY = y;
		x -= prevMousePosX;
		y -= prevmousePosY;
		app->render->currentCamX += x * scale;
		app->render->currentCamY += y * scale;

		prevMousePosX = auxX;
		prevmousePosY = auxY;

	}

	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_STATE::KEY_DOWN)
	{
		app->win->scale -= 0.25f;
		if (app->win->scale < 0.5f)
			app->win->scale = 0.5f;
	}
	if (app->input->GetKey(SDL_SCANCODE_O) == KEY_STATE::KEY_DOWN)
	{
		app->win->scale += 0.25f;
		if (app->win->scale > 5.0f)
			app->win->scale = 5.0f;
	}

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
