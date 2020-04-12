#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Minimap.h"
#include "Map.h"
#include "Textures.h"
#include "Window.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "UIManager.h"
#include "UI.h"

Minimap::Minimap() :minimapLoaded(false)
{
	name = "minimap";
}


Minimap::~Minimap()
{

}


bool Minimap::Awake(pugi::xml_node& config)
{
	width = config.attribute("width").as_int(1);

	position.x = config.attribute("posX").as_int(0);
	position.y = config.attribute("posY").as_int(0);

	return true;
}


// Called before the first frame
bool Minimap::Start()
{

	bool ret = true;

	app->eventManager->EventRegister(EVENT_ENUM::UNPAUSE_GAME_AND_RETURN_TO_MAIN_MENU, this);

	return ret;

}


// Called each loop iteration
bool Minimap::PreUpdate(float dt)
{
	CheckListener(this);
	float UIscale = app->win->GetUIScale();
	UI* element = app->uiManager->FindUIByName("minimapBackground");
	position.x = element->worldPosition.x*UIscale +5;
	position.y = element->worldPosition.y*UIscale + 7;

	return true;
}


// Called each loop iteration
bool Minimap::Update(float dt)
{
	CheckListener(this);


	int x;
	int y;
	int w;
	int h;
	app->input->GetMousePositionRaw(x, y);
	app->render->GetCameraMeasures(w, h);

	float scale = app->win->GetScale();
	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_STATE::KEY_DOWN || app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_STATE::KEY_REPEAT)
	{
		if (ClickingOnMinimap(x, y) == true)
		{
			//camera TP
			iMPoint newCamPos = ScreenToMinimapToWorld(x, y);

			app->render->currentCamX = -((newCamPos.x * scale) - (w * 0.5f));
			app->render->currentCamY = -((newCamPos.y * scale) - (h * 0.5f));

		}
	}

	return true;
}


// Called each loop iteration
bool Minimap::PostUpdate(float dt)
{
	bool ret = true;

	if (minimapLoaded)
	{
		app->render->MinimapBlit(minimapTexture, position.x, position.y, NULL, 1.0);


		//camera rect representation
		SDL_Rect cam;

		float scale = app->win->GetScale();
		int w;
		int h;
		int x = -(app->render->GetCameraX())/scale;
		int y = -(app->render->GetCameraY())/scale;
		app->render->GetCameraMeasures(w, h);

		iMPoint camXY = WorldToMinimap(x, y);
		cam.x = camXY.x;
		cam.y = camXY.y;
		cam.w = w * minimapScaleRelation/scale;
		cam.h = h * minimapScaleRelation/scale;

		//rectangle trim
		//right
		if (cam.x + cam.w > position.x + width)
		{
			cam.w = position.x + width - cam.x;
		}
		//bottom
		if (cam.y + cam.h > position.y + height)
		{
			cam.h = position.y + height - cam.y;
		}
		//left
		if (cam.x < position.x)
		{
			cam.w -= position.x - cam.x;
			cam.x += position.x - cam.x;
		}
		if (cam.y < position.y)
		{
			cam.h -= position.y - cam.y;
			cam.y += position.y - cam.y;
		}

	

		app->render->DrawQuad(cam, 255, 255, 255, 255, false,false);
	}


	return ret;
}


// Called before quitting
bool Minimap::CleanUp()
{
	return true;
}


bool Minimap::Load(pugi::xml_node&)
{
	return true;
}


bool Minimap::Save(pugi::xml_node&) const
{
	return true;
}


void Minimap::ExecuteEvent(EVENT_ENUM eventId)
{
	switch (eventId)
	{
	case EVENT_ENUM::UNPAUSE_GAME_AND_RETURN_TO_MAIN_MENU:
		CleanUp();
		break;
	}
}

void Minimap::CreateMinimapText()
{
	app->map->DrawMinimap();
	app->entityManager->DrawOnlyStaticBuildings();
}

void Minimap::LoadMinimap()
{
	minimapLoaded = true;
	uint windowWidth, windowHeight;
	app->win->GetWindowSize(windowWidth, windowHeight);	

	minimapWidth = app->map->data.tileWidth * app->map->data.width;
	minimapHeight = app->map->data.tileHeight * app->map->data.height;
	minimapScaleRelation = ((width) / ((float)minimapWidth));
	height = (minimapHeight)* minimapScaleRelation;

	minimapTexture = SDL_CreateTexture(app->render->renderer, SDL_GetWindowPixelFormat(app->win->window), SDL_TEXTUREACCESS_TARGET, 1.05f * width, 1.05f * height);

	SDL_SetRenderTarget(app->render->renderer, minimapTexture);
	CreateMinimapText();
	SDL_SetRenderTarget(app->render->renderer, NULL);
}


bool Minimap::ClickingOnMinimap(int x, int y)
{
	bool ret = false;

	SDL_Rect minimap = { position.x, position.y,width,height };

	if ((x > minimap.x) && (x < minimap.x + minimap.w) && (y > minimap.y) && (y < minimap.y + minimap.h))
		ret = true;

	return ret;
}


iMPoint Minimap::WorldToMinimap(int x, int y)
{
	iMPoint minimap_position;
	minimap_position.x = position.x + width * 0.5f + x * minimapScaleRelation;
	minimap_position.y = position.y + y * minimapScaleRelation;

	return minimap_position;
}


iMPoint Minimap::ScreenToMinimapToWorld(int x, int y) {
	iMPoint minimap_position;
	minimap_position.x = (x - position.x - width * 0.5f) / minimapScaleRelation;
	minimap_position.y = (y - position.y) / minimapScaleRelation;
	return minimap_position;
}