#include "App.h"
#include "Input.h"
#include "SDL/include/SDL_scancode.h"
#include "MainMenuScene.h"
#include "FadeToBlack.h"
#include "TestScene.h"
#include "Textures.h"
#include "Audio.h"
#include "UIManager.h"
#include "UIFactory.h"
#include "EntityManager.h"
#include "Render.h"
#include "EventManager.h"

ModuleMainMenuScene::ModuleMainMenuScene() : changeScene(false), changeSceneContinue(-1), fadeTime(0), BG(nullptr), gameIconPosX(0.0f),
canon(0), gameIcon(nullptr), gameTitle(nullptr), soundDelay(0), titleSound(-1)
{
	name.create("menuScene");
}


ModuleMainMenuScene::~ModuleMainMenuScene()
{

}


bool  ModuleMainMenuScene::Awake(pugi::xml_node& config)
{

	app->eventManager->EventRegister(EVENT_ENUM::START_GAME, this);
	app->eventManager->EventRegister(EVENT_ENUM::CREATE_OPTION_MENU, this);
	app->eventManager->EventRegister(EVENT_ENUM::CREATE_CREDIT_MENU, this);

	//sounds
	titleSound = app->audio->LoadFx("Assets/audio/sfx/IntroScene/title.wav");
	fadeTime = config.attribute("fadeTime").as_float(0);

	return true;
}


// Called before the first frame
bool ModuleMainMenuScene::Start()
{

	//images
	gameIcon = app->tex->Load("Assets/intro_images/gameIcon.png");
	gameTitle = app->tex->Load("Assets/intro_images/gameTitle.png");
	BG = app->tex->Load("Assets/intro_images/MainMenuBG.png");

	app->audio->PlayMusic("Assets/audio/music/IntroMenu.ogg", fadeTime, app->audio->musicVolume);

	alphaCounter.NewEasing(EASING_TYPE::EASE_OUT_CUBIC, 1.0, 255.0, 4.0);
	gameIconPosXfunction.NewEasing(EASING_TYPE::EASE_OUT_QUINT, -140, 140, 3.0);
	soundDelay = 0;
	canon = false;

	app->audio->PlayFx(titleSound, 0, 3, LOUDNESS::QUIET, DIRECTION::LEFT);

	changeScene = false;
	changeSceneContinue = -1;


	app->eventManager->GenerateEvent(EVENT_ENUM::CREATE_INTRO_MENU, EVENT_ENUM::NULL_EVENT);

	return true;
}


// Called each loop iteration
bool  ModuleMainMenuScene::PreUpdate(float dt)
{
	CheckListener(this);

	return true;
}


// Called each loop iteration
bool  ModuleMainMenuScene::Update(float dt)
{
	float alpha = 0;
	CheckListener(this);

	if (alphaCounter.IsActive() == true)
	{
		alpha = alphaCounter.UpdateEasingAddingTime(dt);
	}

	if (gameIconPosXfunction.IsActive() == true)
	{
		gameIconPosX = gameIconPosXfunction.UpdateEasingAddingTime(dt);
	}

	if (soundDelay < 210) { soundDelay += dt * 100; }

	app->render->Blit(BG, 0, 0, NULL, false, false, 250);
	app->render->Blit(gameIcon, gameIconPosX, 70, false, false, NULL, alpha);
	app->render->Blit(gameTitle, 20, 20, false, false, NULL, alpha);

	if (soundDelay > 210) 
	{
		if (canon == false) 
		{
			canon = true;
			app->audio->PlayFx(titleSound, 0, 2, LOUDNESS::QUIET, DIRECTION::RIGHT);
		}
	}

	return true;
}


// Called each loop iteration
bool  ModuleMainMenuScene::PostUpdate(float dt)
{
	bool ret = true;

	//TODO CHANGE THIS FOR THE ACTION THAT CHANGES TO THE MAIN SCENE
	if (app->input->GetKey(SDL_SCANCODE_N) == KEY_STATE::KEY_DOWN || changeScene == true)
	{

		if (app->fadeToBlack->FadeToBlack(this, app->testScene, fadeTime * 2))
		{
			changeScene = false;
		}
	}

	if (changeSceneContinue == 0)
	{
		if (app->fadeToBlack->FadeToBlack(this, app->testScene, fadeTime * 2) == true)
		{
			changeSceneContinue = 1;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_STATE::KEY_DOWN) {

		ret = false;
	}

	return ret;
}


// Called before quitting
bool  ModuleMainMenuScene::CleanUp()
{
	app->uiManager->CleanUp();

	app->tex->UnLoad(gameIcon);
	gameIcon = nullptr;
	app->tex->UnLoad(gameTitle);
	gameTitle = nullptr;
	app->tex->UnLoad(BG);
	BG = nullptr;

	if (changeSceneContinue == 1)
	{
		changeSceneContinue = -1;
		app->eventManager->GenerateEvent(EVENT_ENUM::LOAD_GAME, EVENT_ENUM::GAME_SCENE_ENTERED);
	}

	return true;
}


bool  ModuleMainMenuScene::Load(pugi::xml_node&)
{
	return true;
}


bool  ModuleMainMenuScene::Save(pugi::xml_node&) const
{
	return true;
}


void ModuleMainMenuScene::ExecuteEvent(EVENT_ENUM eventId)
{
	switch (eventId)
	{
	case EVENT_ENUM::START_GAME:
		if (app->uiManager->factory != nullptr)
		{
			app->uiManager->factory->ResetUpgradeCost();
		}
		app->entityManager->ResetUpgradeValues();
		changeScene = true;
		break;
	}
}