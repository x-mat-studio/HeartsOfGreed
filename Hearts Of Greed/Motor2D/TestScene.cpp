#include "TestScene.h"
#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"
#include "Audio.h"
#include "Map.h"
#include "Fonts.h"
#include "Pathfinding.h"
#include "FoWManager.h"
#include "EntityManager.h"
#include "FadeToBlack.h"
#include "WinScene.h"
#include "LoseScene.h"
#include "UIManager.h"
#include "UI_Text.h"
#include "MainMenuScene.h"

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
	//Play Music
	app->audio->PlayMusic("audio/music/Map.ogg", 0.0F, 50);
	
	//Load sfx used in this scene
	
	app->uiManager->LoadAtlas();

	if (app->map->LoadNew("map_prototype2.tmx") == true)
	{
		int w, h;
		uchar* data = nullptr;
		if (app->map->CreateWalkabilityMap(w, h, &data))
		{
			app->pathfinding->SetMap(w, h, data);
			RELEASE_ARRAY(data);
		}
		app->fowManager->CreateFoWMap(app->map->data.width, app->map->data.height);
		
		fMPoint pos;
		pos.create(100, 600);

		//Test Hero
		app->entityManager->AddEntity(ENTITY_TYPE::HERO_MELEE, pos.x, pos.y);
		app->entityManager->AddEntity(ENTITY_TYPE::HERO_MELEE, pos.x + 64, pos.y);


		app->entityManager->AddEntity(ENTITY_TYPE::ENEMY, 150, 850);
		app->entityManager->AddEntity(ENTITY_TYPE::ENEMY, 200, 850);
		app->entityManager->AddEntity(ENTITY_TYPE::ENEMY, 250, 850);
	}

	app->uiManager->CreateBasicUI();

	SDL_Rect rect = { 0, 0, 0, 0 };
	app->uiManager->AddUIElement(fMPoint(20, 0), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"TestScene", DRAGGABLE::DRAG_OFF, "DEMO OF TEXT / Test Scene /  Press F to go to the Menu / N to Win / M to Lose");

	
	return true;
}


// Called each loop iteration
bool  ModuleTestScene::PreUpdate(float dt)
{
	CheckListener(this);

	return true;
}


// Called each loop iteration
bool  ModuleTestScene::Update(float dt)
{
	CheckListener(this);

	float scale = app->win->GetScale();
	float camVel = 350 * scale;
	iMPoint mousePos;
	app->input->GetMousePosition(mousePos.x, mousePos.y);
	iMPoint mouseRaw;
	app->input->GetMousePositionRaw(mouseRaw.x, mouseRaw.y);


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
	
	//TODO CHANGE THIS FOR THE ACTION THAT CHANGES TO THE WIN SCENE
	if (app->input->GetKey(SDL_SCANCODE_N) == KEY_STATE::KEY_DOWN)
	{
		app->fadeToBlack->FadeToBlack(this, app->winScene);
	}
	//TODO CHANGE THIS FOR THE ACTION THAT CHANGES TO THE LOSE SCENE
	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_STATE::KEY_DOWN)
	{
		app->fadeToBlack->FadeToBlack(this, app->loseScene);
	}
	//TODO CHANGE THIS FOR THE ACTION THAT CHANGES TO THE MENU SCENE
	if (app->input->GetKey(SDL_SCANCODE_F) == KEY_STATE::KEY_DOWN)
	{
		app->fadeToBlack->FadeToBlack(this, app->mainMenu);
	}


	//mouse drag / mouse zoom
	iMPoint scrollWheel;
	app->input->GetScrollWheelMotion(scrollWheel.x, scrollWheel.y);

	if (app->input->GetMouseButtonDown(2) == KEY_STATE::KEY_DOWN) //TODO THIS WILL BE A START DRAGGING EVENT
	{
		StartDragging(mousePos);
	}
	else if (app->input->GetMouseButtonDown(2) == KEY_STATE::KEY_REPEAT) //TODO THIS WILL BE ACTIVE WHILE STOP DRAGGING EVENT ISN'T SENT
	{
		Drag(mousePos,scale);
	}
	else if (scrollWheel.y != 0)
	{
		//that 0.25 is an arbitrary number and will be changed to be read from the config file. TODO
		Zoom(0.25f * scrollWheel.y, mouseRaw.x, mouseRaw.y, scale);
	}




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
	app->pathfinding->CleanUp();
	app->uiManager->CleanUp();
	app->entityManager->DeleteAllEntities();
	app->coll->CleanUp();
	app->map->CleanUp();
	app->fowManager->DeleteFoWMap();
	app->audio->SilenceAll();

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


void ModuleTestScene::ExecuteEvent(EVENT_ENUM eventId) const
{}


void ModuleTestScene::StartDragging(iMPoint mousePos)
{
	prevMousePosX = mousePos.x;
	prevmousePosY = mousePos.y;
}

void ModuleTestScene::Drag(iMPoint mousePos,float scale)
{
	iMPoint aux = { 0,0 };
	iMPoint xy = mousePos;
	aux = xy;
	xy.x -= prevMousePosX;
	xy.y -= prevmousePosY;
	app->render->currentCamX += xy.x * scale;
	app->render->currentCamY += xy.y * scale;

	prevMousePosX = aux.x;
	prevmousePosY = aux.y;
}