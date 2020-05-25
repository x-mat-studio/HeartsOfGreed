#include "App.h"
#include "Input.h"
#include "SDL/include/SDL_scancode.h"
#include "WinScene.h"
#include "FadeToBlack.h"
#include "MainMenuScene.h"
#include "UIManager.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "EasingFunctions.h"

ModuleWinScene::ModuleWinScene(): fadeTime(0)
{
	name.create("winScene");
}


ModuleWinScene::~ModuleWinScene()
{

}


bool  ModuleWinScene::Awake(pugi::xml_node&config)
{

	medalPos.x = config.attribute("medalPosX").as_int(0);
	medalPos.y = config.attribute("medalPosY").as_int(0);
	fadeTime = config.attribute("fadeTime").as_float(0);

	return true;
}


// Called before the first frame
bool ModuleWinScene::Start()
{
	SDL_Rect rect = { 0, 0, 0, 0 };

	youWon = app->tex->Load("intro_images/youWon.png");
	medalWin = app->tex->Load("intro_images/medalWin.png");

	app->audio->PlayMusic("audio/music/youWon.ogg", 3*fadeTime, app->audio->musicVolume);

	iconPosY.NewEasing(EASING_TYPE::EASE_OUT_BOUNCE, medalPos.y - 300, medalPos.y, 2.0);


	return true;
}


// Called each loop iteration
bool  ModuleWinScene::PreUpdate(float dt)
{
	CheckListener(this);

	return true;
}


// Called each loop iteration
bool  ModuleWinScene::Update(float dt)
{
	CheckListener(this);
	iconPosY.UpdateEasingAddingTime(dt);

	app->render->Blit(youWon,0,0, NULL, false, false);
	app->render->Blit(medalWin, medalPos.x, iconPosY.GetLastRequestedPos(), NULL, false, false);

	return true;
}


// Called each loop iteration
bool  ModuleWinScene::PostUpdate(float dt)
{
	bool ret = true;

	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_STATE::KEY_DOWN) {

		app->fadeToBlack->FadeToBlack(this, app->mainMenu, fadeTime * 2);
		iconPosY.NewEasing(EASING_TYPE::EASE_IN_SINE, medalPos.y, medalPos.y + 1000, 2.0);
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_STATE::KEY_DOWN) {

		ret = false;
	}
	return ret;
}


// Called before quitting
bool  ModuleWinScene::CleanUp()
{
	app->uiManager->CleanUp();
	app->tex->UnLoad(youWon);
	youWon = nullptr;
	app->tex->UnLoad(medalWin);
	medalWin = nullptr;
	return true;
}


bool  ModuleWinScene::Load(pugi::xml_node&)
{
	return true;
}


bool  ModuleWinScene::Save(pugi::xml_node&) const
{
	return true;
}


void ModuleWinScene::ExecuteEvent(EVENT_ENUM eventId) const
{}