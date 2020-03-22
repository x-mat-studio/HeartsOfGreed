#include "TestScene.h"
#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"
#include "Audio.h"
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
	
	app->audio->PlayMusic("audio/music/Raiders.ogg", 0.0F, 50);
	
	return true;
}


// Called each loop iteration
bool  ModuleTestScene::PreUpdate(float dt)
{
	CheckListener();

	return true;
}


// Called each loop iteration
bool  ModuleTestScene::Update(float dt)
{
	CheckListener();

	float scale = app->win->GetScale();
	float camVel = 10 * scale;
	int mousePosX;
	int mousePosY;
	app->input->GetMousePosition(mousePosX, mousePosY);
	int mouseRawX;
	int mouseRawY;
	app->input->GetMousePositionRaw(mouseRawX, mouseRawY);


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
	
	

	//mouse drag / mouse zoom
	int scrollWheelX;
	int scrollWheelY;
	app->input->GetScrollWheelMotion(scrollWheelX, scrollWheelY);


	/*if (app->input->GetMouseButtonDown(1) == KEY_STATE::KEY_DOWN || app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_STATE::KEY_DOWN)
	{
		prevMousePosX = mousePosX;
		prevmousePosY = mousePosY;
	}
	else if (app->input->GetMouseButtonDown(1) == KEY_STATE::KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_STATE::KEY_REPEAT)
	{
		int auxX = 0;
		int auxY = 0;
		int x = mousePosX;
		int y = mousePosY;
		auxX = x;
		auxY = y;
		x -= prevMousePosX;
		y -= prevmousePosY;
		app->render->currentCamX += x * scale;
		app->render->currentCamY += y * scale;

		prevMousePosX = auxX;
		prevmousePosY = auxY;

	}
	else if (scrollWheelY != 0)
	{
		//that 0.25 is an arbitrary number and will be changed to be read from the config file. TODO
		Zoom(0.25f * scrollWheelY, mouseRawX, mouseRawY, scale);
	}*/

	



	return true;
}


// Called each loop iteration
bool  ModuleTestScene::PostUpdate(float dt)
{
	bool ret = true;
	
	app->map->Draw();
	
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_STATE::KEY_DOWN) {

		ret = false;
	}

	return ret;
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


void ModuleTestScene::Zoom(float addZoomAmount, int windowTargetCenterX, int windowTargetCenterY, float currentScale)
{
	float newScale = app->win->AddScale(addZoomAmount);
	float increment = newScale - currentScale;
	float offsetX = windowTargetCenterX;
	float offsetY = windowTargetCenterY;

	//does not change the camera if there is no scaling
	if (increment != 0.0f)
	{
		app->render->currentCamX = (((app->render->currentCamX - offsetX) * newScale) / currentScale) + offsetX;
		app->render->currentCamY = (((app->render->currentCamY - offsetY) * newScale) / currentScale) + offsetY;
	}
}


void ModuleTestScene::ExecuteEvent(EVENT_ENUM& eventId) const
{}