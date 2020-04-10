#include "App.h"
#include "Input.h"
#include "SDL/include/SDL_scancode.h"
#include "IntroScene.h"
#include "FadeToBlack.h"
#include "MainMenuScene.h"
#include "UIManager.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "UI_Text.h"


ModuleIntroScene::ModuleIntroScene()
{

}


ModuleIntroScene::~ModuleIntroScene()
{

}


bool  ModuleIntroScene::Awake(pugi::xml_node&)
{
	return true;
}


// Called before the first frame
bool ModuleIntroScene::Start()
{
	AlphaCounter = 0;
	bgAlphaCounter = 255;
	
	SDL_Rect rect = { 0, 0, 0, 0 };
	app->uiManager->AddUIElement(fMPoint(20, 0), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"IntroScene", DRAGGABLE::DRAG_OFF, "DEMO OF TEXT / Intro Scene  /  Press N to go to the Menu");


	//images
	// gameIcon = app->tex->Load("intro_images/gameIcon.png");
	// gameTitle = app->tex->Load("intro_images/gameTitle.png");
	logoXMat = app->tex->Load("intro_images/logoXMat.png");
	logoBG = app->tex->Load("intro_images/logoBG.png");
	
	//sounds
	logoSound = app->audio->LoadFx("audio/sfx/IntroScene/Logo_sfx.wav");
	// titleSound = app->audio->LoadFx("audio/sfx/IntroScene/Logo_sfx.wav");


	app->audio->PlayFx(logoSound);
	return true;
}


// Called each loop iteration
bool  ModuleIntroScene::PreUpdate(float dt)
{
	CheckListener(this);

	return true;
}


// Called each loop iteration
bool  ModuleIntroScene::Update(float dt)
{
	CheckListener(this);

	return true;
}


// Called each loop iteration
bool  ModuleIntroScene::PostUpdate(float dt)
{
	bool ret = true;

	//TODO CHANGE THIS FOR THE ACTION THAT CHANGES TO THE MAIN MENU
	if (app->input->GetKey(SDL_SCANCODE_N) == KEY_STATE::KEY_DOWN) {

		app->fadeToBlack->FadeToBlack(this, app->mainMenu, 2.0f);
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_STATE::KEY_DOWN) {

		ret = false;
	}
	if (AlphaCounter < 250) {

		AlphaCounter += dt * 75;
	}
	if (bgAlphaCounter > 130) {

		bgAlphaCounter -= dt * 50;
	}

	app->render->Blit(logoBG, 0, 0, false, false,false,bgAlphaCounter);

	if (AlphaCounter > 1) {
		
		app->render->Blit(logoXMat, 155, 20, false, false, NULL, AlphaCounter);
	}
	

	return ret;
}


// Called before quitting
bool  ModuleIntroScene::CleanUp()
{
	app->tex->UnLoad(logoXMat);
	app->tex->UnLoad(logoBG);
	logoXMat = nullptr;
	logoBG = nullptr;
	
	
	app->uiManager->CleanUp();

	return true;
}


bool  ModuleIntroScene::Load(pugi::xml_node&)
{
	return true;
}


bool  ModuleIntroScene::Save(pugi::xml_node&) const
{
	return true;
}


void ModuleIntroScene::ExecuteEvent(EVENT_ENUM eventId) const
{}