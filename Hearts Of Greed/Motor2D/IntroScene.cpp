#include "App.h"
#include "Input.h"
#include "SDL/include/SDL_scancode.h"
#include "IntroScene.h"
#include "FadeToBlack.h"
#include "MainMenuScene.h"
#include "UIManager.h"
#include "Textures.h"
#include "Audio.h"
#include "Video.h"
#include "Window.h"
#include "Render.h"



ModuleIntroScene::ModuleIntroScene() : bgValueDown(false), logoValueDown(false), logoAlphaCounter(0), bgStartingValue(0), bgTopValue(0),
bgTransitionConst(0), logoBG(nullptr), bgAlphaCounter(0), logoSound(-1), logoStartingValue(0), logoTopValue(0), logoTransitionConst(0),
logoXMat(nullptr), titleSound(-1)
{
	name.create("introScene");


}


ModuleIntroScene::~ModuleIntroScene()
{

}


bool  ModuleIntroScene::Awake(pugi::xml_node& config)
{
	logoStartingValue = config.attribute("logoStartingAlphaValue").as_float(0);
	logoStartingValue = MAX(logoStartingValue, 0);
	logoStartingValue = MIN(logoStartingValue, 255);

	bgStartingValue = config.attribute("backgroundStartingAlphaValue").as_float(0);;
	bgStartingValue = MAX(bgStartingValue, 0);
	bgStartingValue = MIN(bgStartingValue, 255);

	logoTopValue = config.attribute("logoFinalAlphaValue").as_float(0);
	logoTopValue = MAX(logoTopValue, 0);
	logoTopValue = MIN(logoTopValue, 255);

	bgTopValue = config.attribute("backgroundFinalAlphaValue").as_float(0);
	bgTopValue = MAX(bgTopValue, 0);
	bgTopValue = MIN(bgTopValue, 255);

	logoTransitionConst = abs(config.attribute("logoTransitionConstant").as_float(0));
	logoTransitionConst = MAX(logoTransitionConst, 1);
	logoTransitionConst = MIN(logoTransitionConst, 255);

	bgTransitionConst = abs(config.attribute("backgroundTransitionConstant").as_float(0));
	bgTransitionConst = MAX(bgTransitionConst, 1);
	bgTransitionConst = MIN(bgTransitionConst, 255);


	if (logoTopValue - logoStartingValue < 0)
	{
		logoTransitionConst *= -1;
		logoValueDown = true;
	}
	else if (logoTopValue - logoStartingValue == 0)
	{
		logoTransitionConst = 0;
	}


	if (bgTopValue - bgStartingValue < 0)
	{
		bgTransitionConst *= -1;
		bgValueDown = true;
	}
	else if (bgTopValue - bgStartingValue == 0)
	{
		bgTransitionConst = 0;
	}

	app->win->SetScale(1.00f);
	return true;
}


// Called before the first frame
bool ModuleIntroScene::Start()
{
	//logoAlphaCounter = logoStartingValue;
	//bgAlphaCounter = bgStartingValue;

	//SDL_Rect rect = { 0, 0, 0, 0 };
	////app->uiManager->AddUIElement(fMPoint(20, 0), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"IntroScene", nullptr, DRAGGABLE::DRAG_OFF, "DEMO OF TEXT / Intro Scene  /  Press N to go to the Menu");


	////images
	//logoXMat = app->tex->Load("intro_images/logoXMat.png");
	//logoBG = app->tex->Load("intro_images/logoBG.png");

	////sounds
	//logoSound = app->audio->LoadFx("audio/sfx/IntroScene/Logo_sfx.wav");

	app->video->Initialize("video/introAVI.avi");

	//app->audio->PlayFx(logoSound);
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

	/*if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_STATE::KEY_DOWN) 
	{

		ret = false;
	}

	logoAlphaCounter += dt * logoTransitionConst;


	bgAlphaCounter += dt * bgTransitionConst;


	app->render->Blit(logoBG, 0, 0, false, false, false, bgAlphaCounter);



	if (logoValueDown == true)
	{
		logoAlphaCounter = MAX(logoAlphaCounter, logoTopValue);
	}
	else
	{
		logoAlphaCounter = MIN(logoAlphaCounter, logoTopValue);
	}

	if (bgValueDown == true)
	{
		bgAlphaCounter = MAX(bgAlphaCounter, bgTopValue);
	}
	else
	{
		bgAlphaCounter = MIN(bgAlphaCounter, bgTopValue);
	}


	if (logoAlphaCounter > 1) {

		app->render->Blit(logoXMat, 155, 20, false, false, NULL, logoAlphaCounter);
	}*/

	if (app->video->isVideoFinished)
	{
		app->fadeToBlack->FadeToBlack(this, app->mainMenu, 2.0f);
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

	app->win->SetScale(2.0f);
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