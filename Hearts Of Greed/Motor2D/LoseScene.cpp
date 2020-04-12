#include "App.h"
#include "Input.h"
#include "SDL/include/SDL_scancode.h"
#include "LoseScene.h"
#include "FadeToBlack.h"
#include "MainMenuScene.h"
#include "UIManager.h"
#include "UI_Text.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"

ModuleLoseScene::ModuleLoseScene()
{}


ModuleLoseScene::~ModuleLoseScene()
{}


bool  ModuleLoseScene::Awake(pugi::xml_node&)
{
	return true;
}


// Called before the first frame
bool ModuleLoseScene::Start()
{
	SDL_Rect rect = { 0, 0, 0, 0 };
	//app->uiManager->AddUIElement(fMPoint(20, 0), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"LoseScene", DRAGGABLE::DRAG_OFF, "DEMO OF TEXT / Lose Scene /  Press N to go to the Menu");

	youLost = app->tex->Load("intro_images/youLost.png");
	medalLose = app->tex->Load("intro_images/medalLose.png");

	app->audio->PlayMusic("audio/music/youLost.ogg", 15.0F, 200);

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

	app->render->Blit(youLost, -42, 0);
	app->render->Blit(medalLose, 30, 0);

	return true;
}


// Called each loop iteration
bool  ModuleLoseScene::PostUpdate(float dt)
{
	bool ret = true;
	//TODO CHANGE THIS FOR THE ACTION THAT CHANGES TO THE MAIN MENU
	if (app->input->GetKey(SDL_SCANCODE_N) == KEY_STATE::KEY_DOWN) {

		app->fadeToBlack->FadeToBlack(this, app->mainMenu, 2.0f);
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