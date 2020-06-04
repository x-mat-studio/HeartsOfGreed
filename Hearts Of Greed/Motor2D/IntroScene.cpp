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
	app->win->SetScale(1.00f);
	return true;
}


// Called before the first frame
bool ModuleIntroScene::Start()
{
	app->video->Initialize("data/video/introAVI.avi");
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

	if ((app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_STATE::KEY_DOWN) || app->input->GetMouseButtonDown(1) == KEY_STATE::KEY_DOWN)
	{	
		app->fadeToBlack->FadeToBlack(this, app->mainMenu, 1.0f);
	}

	if (app->video->isVideoFinished)
	{
		app->fadeToBlack->FadeToBlack(this, app->mainMenu, 1.0f);
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