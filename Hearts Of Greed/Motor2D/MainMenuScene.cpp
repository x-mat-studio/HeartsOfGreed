#include "App.h"
#include "Input.h"
#include "SDL/include/SDL_scancode.h"
#include "MainMenuScene.h"
#include "FadeToBlack.h"
#include "TestScene.h"
#include "Textures.h"
#include "Audio.h"
#include "UIManager.h"
#include "Render.h"
#include "UI_Text.h"
#include "EventManager.h"

ModuleMainMenuScene::ModuleMainMenuScene() : changeScene(false),changeSceneContinue(-1), fadeTime(0)
{
	name.create("menuScene");
}


ModuleMainMenuScene::~ModuleMainMenuScene()
{

}


bool  ModuleMainMenuScene::Awake(pugi::xml_node&config)
{

	app->eventManager->EventRegister(EVENT_ENUM::START_GAME, this);
	app->eventManager->EventRegister(EVENT_ENUM::START_GAME_FROM_CONTINUE, this);
	app->eventManager->EventRegister(EVENT_ENUM::OPTION_MENU, this);
	app->eventManager->EventRegister(EVENT_ENUM::CREDIT_MENU, this);

	//sounds
	titleSound = app->audio->LoadFx("audio/sfx/IntroScene/title.wav");
	fadeTime = config.attribute("fadeTime").as_float(0);

	return true;
}


// Called before the first frame
bool ModuleMainMenuScene::Start()
{

	//SDL_Rect rect = { 0, 0, 0, 0 };
	//app->uiManager->AddUIElement(fMPoint(20, 0), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"MenuScene", DRAGGABLE::DRAG_OFF, "DEMO OF TEXT / Menu Scene /  Press N to go to the Game");

	app->uiManager->CreateMainMenu();

	//images
	gameIcon = app->tex->Load("intro_images/gameIcon.png");
	gameTitle = app->tex->Load("intro_images/gameTitle.png");
	BG = app->tex->Load("intro_images/MainMenuBG.png");

	app->audio->PlayMusic("audio/music/IntroMenu.ogg", fadeTime, 200);

	alphaCounter = 0;
	soundDelay = 0;
	canon = false;

	app->audio->PlayFx(titleSound, 0, 3, LOUDNESS::QUIET, DIRECTION::LEFT);

	changeScene = false;
	changeSceneContinue = -1;
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
	CheckListener(this);

	if (alphaCounter < 255) { alphaCounter += dt * 70; }
	if (soundDelay < 210) { soundDelay += dt * 100; }

	app->render->Blit(BG, 0,0, NULL, false, false, 250);
	app->render->Blit(gameIcon, 140, 70, false, false, NULL, alphaCounter);
	app->render->Blit(gameTitle, 20, 20, false, false, NULL, alphaCounter);

	if (soundDelay > 210) {

		if (canon == false) {
			
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

		if (app->fadeToBlack->FadeToBlack(this, app->testScene,fadeTime*2))
		{
			changeScene = false;
		}
	}

	if (changeSceneContinue == 0)
	{
		if (app->fadeToBlack->FadeToBlack(this, app->testScene, fadeTime * 2)==true)
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
		app->eventManager->GenerateEvent(EVENT_ENUM::LOAD_GAME, EVENT_ENUM::GAME_SCENE_STARTED);
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
		changeScene = true;
		break;
	case EVENT_ENUM::START_GAME_FROM_CONTINUE:
		changeSceneContinue = 0;
		break;
	}
}