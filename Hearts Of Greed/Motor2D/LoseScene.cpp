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

ModuleLoseScene::ModuleLoseScene():fadeTime(0)
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

	return true;
}


// Called before the first frame
bool ModuleLoseScene::Start()
{
	SDL_Rect rect = { 0, 0, 0, 0 };

	/*youLost = app->tex->Load("intro_images/youLost.png");
	medalLose = app->tex->Load("intro_images/medalLose.png");*/

	app->audio->PlayMusic("audio/music/youLost.ogg", fadeTime, app->audio->musicVolume);

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
	CheckListener(this);

	app->render->Blit(youLost, -42, 0,NULL, false,false);
	app->render->Blit(medalLose, medalPos.x, medalPos.y, NULL, false, false);

	return true;
}


// Called each loop iteration
bool  ModuleLoseScene::PostUpdate(float dt)
{
	bool ret = true;

	
	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_STATE::KEY_DOWN) {

		app->fadeToBlack->FadeToBlack(this, app->mainMenu, fadeTime * 2);
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
	return true;
}


bool  ModuleLoseScene::Load(pugi::xml_node&)
{
	return true;
}


bool  ModuleLoseScene::Save(pugi::xml_node&) const
{
	return true;
}


void ModuleLoseScene::ExecuteEvent(EVENT_ENUM eventId) const
{}