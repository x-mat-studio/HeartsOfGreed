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
#include "EventManager.h"

ModuleTestScene::ModuleTestScene() :
	prevMousePosX(0),
	prevmousePosY(0),
	camUp(false),
	camDown(false),
	camRight(false),
	camLeft(false),
	camSprint(false),
	allowCamMovement(true),
	menuScene(false)
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
		app->entityManager->AddEntity(ENTITY_TYPE::HERO_GATHERER, pos.x-680, pos.y);
		app->entityManager->AddEntity(ENTITY_TYPE::HERO_MELEE, pos.x - 664, pos.y);


		app->entityManager->AddEntity(ENTITY_TYPE::ENEMY, 150, 750);
		app->entityManager->AddEntity(ENTITY_TYPE::ENEMY, 200, 750);
		app->entityManager->AddEntity(ENTITY_TYPE::ENEMY, 250, 750);

		// Test Turret
	//	app->entityManager->AddEntity(ENTITY_TYPE::BLDG_TURRET, 100, 750);
	}

	app->uiManager->CreateBasicInGameUI();

	SDL_Rect rect = { 0, 0, 0, 0 };
	app->uiManager->AddUIElement(fMPoint(20, 0), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"TestScene", DRAGGABLE::DRAG_OFF, "DEMO OF TEXT / Test Scene /  Press F to go to the Menu / N to Win / M to Lose");


	//Events register
	app->eventManager->EventRegister(EVENT_ENUM::CAMERA_UP, this);
	app->eventManager->EventRegister(EVENT_ENUM::CAMERA_DOWN, this);
	app->eventManager->EventRegister(EVENT_ENUM::CAMERA_LEFT, this);
	app->eventManager->EventRegister(EVENT_ENUM::CAMERA_RIGHT, this);
	app->eventManager->EventRegister(EVENT_ENUM::STOP_CAMERA_UP, this);
	app->eventManager->EventRegister(EVENT_ENUM::STOP_CAMERA_DOWN, this);
	app->eventManager->EventRegister(EVENT_ENUM::STOP_CAMERA_LEFT, this);
	app->eventManager->EventRegister(EVENT_ENUM::STOP_CAMERA_RIGHT, this);
	app->eventManager->EventRegister(EVENT_ENUM::CAMERA_SPRINT, this);
	app->eventManager->EventRegister(EVENT_ENUM::STOP_CAMERA_SPRINT, this);
	app->eventManager->EventRegister(EVENT_ENUM::SAVE_GAME, this);
	app->eventManager->EventRegister(EVENT_ENUM::LOAD_GAME, this);
	app->eventManager->EventRegister(EVENT_ENUM::GAME_SCENE_STARTED, this);
	app->eventManager->EventRegister(EVENT_ENUM::PAUSE_GAME, this);
	app->eventManager->EventRegister(EVENT_ENUM::UNPAUSE_GAME, this);
	app->eventManager->EventRegister(EVENT_ENUM::RETURN_TO_MAIN_MENU, this);

	app->eventManager->GenerateEvent(EVENT_ENUM::GAME_SCENE_STARTED, EVENT_ENUM::NULL_EVENT);

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
	float camVel = 700; //TODO LOAD THIS FROM XML
	iMPoint mousePos;
	iMPoint mouseRaw;
	app->input->GetMousePosition(mousePos.x, mousePos.y);
	app->input->GetMousePositionRaw(mouseRaw.x, mouseRaw.y);

	if (app->input->GetKey(SDL_SCANCODE_9) == KEY_STATE::KEY_DOWN) //Debug key to lock camera movement
	{
		ToggleCamMovement();
	}


	if (allowCamMovement)
	{
		bool wasdMove = false;

		if (camSprint)
		{
			camVel *= 2;
			wasdMove = true;
		}
		if (camUp)
		{
			app->render->currentCamY += camVel * dt;
			wasdMove = true;
		}
		if (camDown)
		{
			app->render->currentCamY -= camVel * dt;
			wasdMove = true;
		}
		if (camLeft)
		{
			app->render->currentCamX += camVel * dt;
			wasdMove = true;
		}
		if (camRight)
		{
			app->render->currentCamX -= camVel * dt;
			wasdMove = true;
		}


		if (!wasdMove)
		{
			//mouse drag / mouse zoom
			iMPoint scrollWheel;
			app->input->GetScrollWheelMotion(scrollWheel.x, scrollWheel.y);
			if (MouseCameraDisplacement(camVel, dt) == false)
			{
				if (app->input->GetMouseButtonDown(2) == KEY_STATE::KEY_DOWN) //TODO THIS WILL BE A START DRAGGING EVENT
				{
					StartDragging(mousePos);
				}
				else if (app->input->GetMouseButtonDown(2) == KEY_STATE::KEY_REPEAT) //TODO THIS WILL BE ACTIVE WHILE STOP DRAGGING EVENT ISN'T SENT
				{
					Drag(mousePos, scale);
				}
				else if (scrollWheel.y != 0)
				{
					//that 0.25 is an arbitrary number and will be changed to be read from the config file. TODO
					Zoom(0.25f * scrollWheel.y, mouseRaw.x, mouseRaw.y, scale);
				}
			}
		}
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
	if (app->input->GetKey(SDL_SCANCODE_F) == KEY_STATE::KEY_DOWN || menuScene == true)
	{
		app->fadeToBlack->FadeToBlack(this, app->mainMenu);
		menuScene = false;
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


void ModuleTestScene::ExecuteEvent(EVENT_ENUM eventId)
{
	switch (eventId)
	{
	case EVENT_ENUM::CAMERA_UP:
		camUp = true;
		break;
	case EVENT_ENUM::STOP_CAMERA_UP:
		camUp = false;
		break;
	case EVENT_ENUM::CAMERA_DOWN:
		camDown = true;
		break;
	case EVENT_ENUM::STOP_CAMERA_DOWN:
		camDown = false;
		break;
	case EVENT_ENUM::CAMERA_RIGHT:
		camRight = true;
		break;
	case EVENT_ENUM::STOP_CAMERA_RIGHT:
		camRight = false;
		break;
	case EVENT_ENUM::CAMERA_LEFT:
		camLeft = true;
		break;
	case EVENT_ENUM::STOP_CAMERA_LEFT:
		camLeft = false;
		break;
	case EVENT_ENUM::CAMERA_SPRINT:
		camSprint = true;
		break;
	case EVENT_ENUM::STOP_CAMERA_SPRINT:
		camSprint = false;
		break;
	case EVENT_ENUM::SAVE_GAME:
		// TODO Save game from here
		break;
	case EVENT_ENUM::LOAD_GAME:
		// TODO Load game from here
		break;
	case EVENT_ENUM::PAUSE_GAME:
		app->gamePause = true;
		break;
	case EVENT_ENUM::UNPAUSE_GAME:
		app->gamePause = false;
		break;
	case EVENT_ENUM::RETURN_TO_MAIN_MENU:
		menuScene = true;
		break;
	}

}


void ModuleTestScene::StartDragging(iMPoint mousePos)
{
	prevMousePosX = mousePos.x;
	prevmousePosY = mousePos.y;
}

void ModuleTestScene::Drag(iMPoint mousePos, float scale)
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

bool ModuleTestScene::MouseCameraDisplacement(float camVel, float dt)
{
	bool ret = false;
	int offset = 30; //TODO THIS OFFSET VALUE (IN PIXELS) FOR THE DETECTION WILL BE LOADED FROM XML IN THE FUTURE
	iMPoint mouseRaw;
	uint width;
	uint height;
	app->input->GetMousePositionRaw(mouseRaw.x, mouseRaw.y);
	app->win->GetWindowSize(width, height);

	if (mouseRaw.x <= offset)
	{
		app->render->currentCamX += camVel * dt;
		ret = true;
	}
	else if (mouseRaw.x >= width - 1 - offset)
	{
		app->render->currentCamX -= camVel * dt;
		ret = true;
	}

	if (mouseRaw.y <= offset)
	{
		app->render->currentCamY += camVel * dt;
		ret = true;

	}
	else if (mouseRaw.y >= height - 1 - offset)
	{
		app->render->currentCamY -= camVel * dt;
		ret = true;
	}
	return ret;
}


void ModuleTestScene::ToggleCamMovement()
{
	allowCamMovement = !allowCamMovement;
}


void ModuleTestScene::ActivateCamMovement()
{
	allowCamMovement = true;
}


void ModuleTestScene::DeactivateCamMovement()
{
	allowCamMovement = false;
}


bool ModuleTestScene::GetCamMovementActivated() const
{
	return allowCamMovement;
}

