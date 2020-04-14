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
#include "UI_Text.h"
#include "Window.h"

ModuleWinScene::ModuleWinScene()
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

	return true;
}


// Called before the first frame
bool ModuleWinScene::Start()
{
	SDL_Rect rect = { 0, 0, 0, 0 };
	app->uiManager->AddUIElement(fMPoint(10, app->win->height / app->win->GetUIScale() - 40), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"message", nullptr, DRAGGABLE::DRAG_OFF, "CLICK ANYWHERE TO CONTINUE");
	
	youWon = app->tex->Load("intro_images/youWon.png");
	medalWin = app->tex->Load("intro_images/medalWin.png");

	app->audio->PlayMusic("audio/music/youWon.ogg", 15.0F, 200);

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

	app->render->Blit(youWon,0,0, NULL, false, false);
	app->render->Blit(medalWin, medalPos.x, medalPos.y, NULL, false, false);

	return true;
}


// Called each loop iteration
bool  ModuleWinScene::PostUpdate(float dt)
{
	bool ret = true;

	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_STATE::KEY_DOWN) {

		app->fadeToBlack->FadeToBlack(this, app->mainMenu, 2.0f);
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