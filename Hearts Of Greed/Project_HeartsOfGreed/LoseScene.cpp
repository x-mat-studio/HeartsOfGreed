#include "App.h"
#include "Input.h"
#include "SDL/include/SDL_scancode.h"
#include "LoseScene.h"
#include "FadeToBlack.h"
#include "MainMenuScene.h"
#include "UIManager.h"

#include "Render.h"
#include "Textures.h"
#include "Window.h"
#include "Audio.h"
#include "EasingFunctions.h"

ModuleLoseScene::ModuleLoseScene() : fadeTime(0), youLost(nullptr), medalLose(nullptr), medalRest(nullptr), medalBounce(-1), bufferPos(-1.f), backToMain(false)
{
	name.create("loseScene");
}


ModuleLoseScene::~ModuleLoseScene()
{}


bool  ModuleLoseScene::Awake(pugi::xml_node& config)
{
	medalPos.x = config.attribute("medalPosX").as_int(0);
	medalPos.y = config.attribute("medalPosY").as_int(0);
	fadeTime = config.attribute("fadeTime").as_float(0);

	bufferPos = 0.0;
	medalRest = false;
	backToMain = false;
	return true;
}


// Called before the first frame
bool ModuleLoseScene::Start()
{
	SDL_Rect rect = { 0, 0, 0, 0 };

	youLost = app->tex->Load("Assets/intro_images/youLost.png");
	medalLose = app->tex->Load("Assets/intro_images/medalLose.png");

	app->audio->PlayMusic("Assets/audio/music/youLost.ogg", 3*fadeTime, app->audio->musicVolume);
	medalBounce = app->audio->LoadFx("Assets/audio/sfx/WinLose/MedalSound.wav");

	iconPosY.NewEasing(EASING_TYPE::EASE_OUT_BOUNCE, medalPos.y - 300.0, medalPos.y, 2.0);

	return true;
}


// Called each loop iteration
bool  ModuleLoseScene::PreUpdate(float dt)
{
	CheckListener(this);

	return true;
}


// Called each loop iteration
bool  ModuleLoseScene::Update(float dt)
{
	bufferPos = iconPosY.GetLastRequestedPos();
	
	CheckListener(this);
	iconPosY.UpdateEasingAddingTime(dt);

	app->render->Blit(youLost, -42, 0,NULL, false,false);
	app->render->Blit(medalLose, medalPos.x, iconPosY.GetLastRequestedPos(), NULL, false, false);

	if (iconPosY.GetLastRequestedPos() < bufferPos) {
		app->audio->PlayFx(medalBounce,0,-1,LOUDNESS::NORMAL);
	}
	else if (iconPosY.GetLastRequestedPos() == bufferPos && medalRest == false) {
	
		medalRest = true;
		app->audio->PlayFx(medalBounce, 0, -1, LOUDNESS::NORMAL);
	}

	return true;
}


// Called each loop iteration
bool  ModuleLoseScene::PostUpdate(float dt)
{
	bool ret = true;

	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_STATE::KEY_DOWN && backToMain == false && app->fadeToBlack->NotFadingToBlack() == true) {

		backToMain = true;
		app->fadeToBlack->FadeToBlack(this, app->mainMenu, fadeTime * 2);
		iconPosY.NewEasing(EASING_TYPE::EASE_IN_SINE, medalPos.y, medalPos.y + 1000.0, 2.0);
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_STATE::KEY_DOWN) {

		ret = false;
	}
	return ret;
}


// Called before quitting
bool  ModuleLoseScene::CleanUp()
{
	app->uiManager->CleanUp();
	app->tex->UnLoad(youLost);
	youLost = nullptr;
	app->tex->UnLoad(medalLose);
	medalLose = nullptr;
	medalRest = false;
	backToMain = false;
	return true;
}
