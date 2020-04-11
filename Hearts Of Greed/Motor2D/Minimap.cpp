#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Minimap.h"
#include "Map.h"
#include "Textures.h"
#include "Window.h"

Minimap::Minimap():minimapLoaded(false)
{
	name = "minimap";
}


Minimap::~Minimap()
{

}


bool  Minimap::Awake(pugi::xml_node& config)
{
	width = config.attribute("width").as_int(1);
	
	position.x= config.attribute("posX").as_int(0);
	position.y = config.attribute("posY").as_int(0);

	return true;
}


// Called before the first frame
bool Minimap::Start()
{

	bool ret = true;
	return ret;

}


// Called each loop iteration
bool  Minimap::PreUpdate(float dt)
{
	CheckListener(this);

	return true;
}


// Called each loop iteration
bool  Minimap::Update(float dt)
{
	CheckListener(this);

	return true;
}


// Called each loop iteration
bool  Minimap::PostUpdate(float dt)
{
	bool ret = true;

	if(minimapLoaded)
	app->render->MinimapBlit(minimapTexture, position.x, position.y, NULL, 1.0);

	return ret;
}


// Called before quitting
bool  Minimap::CleanUp()
{
	return true;
}


bool  Minimap::Load(pugi::xml_node&)
{
	return true;
}


bool  Minimap::Save(pugi::xml_node&) const
{
	return true;
}


void Minimap::ExecuteEvent(EVENT_ENUM eventId) const
{

}

void Minimap::CreateMinimapText()
{
	app->map->DrawMinimap();
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
