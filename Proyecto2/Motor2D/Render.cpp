#include "p2Defs.h"
#include "p2Log.h"
#include "App.h"
#include "Window.h"
#include "Render.h"
#include "Collision.h"


ModuleRender::ModuleRender() : Module(), background({ 0,0,0,0 })
{
	name.create("renderer");
}


// Destructor
ModuleRender::~ModuleRender()
{}


// Called before render is available
bool ModuleRender::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");
	bool ret = true;
	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;
	app->vSyncActivated = false;


	if (config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
		app->vSyncActivated = true;
	}


	renderer = SDL_CreateRenderer(app->win->window, -1, flags);


	if (renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		AssignCameraMeasures();
		camera.x = 0;
		camera.y = 0;
	}


	return ret;
}


// Called before the first frame
bool ModuleRender::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);
	currentCamX = camera.x;
	currentCamY = camera.y;

	//these 2 lines are here to test the 1st map TODO delete
	/*app->render->currentCamX = 1027;
	app->render->currentCamY = -2500;*/
	return true;
}


// Called each loop iteration
bool ModuleRender::PreUpdate(float dt)
{
	SDL_RenderClear(renderer);
	return true;
}


bool ModuleRender::Update(float dt)
{
	return true;
}


bool ModuleRender::PostUpdate(float dt)
{
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	
	SDL_RenderPresent(renderer);
	
	return true;
}


// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}


// Load Game State
bool ModuleRender::Load(pugi::xml_node& data)
{
	currentCamX = data.child("camera").attribute("x").as_int(0);
	currentCamY = data.child("camera").attribute("y").as_int(0);

	return true;
}


// Save Game State
bool ModuleRender::Save(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.append_child("camera");

	cam.append_attribute("x") = camera.x;
	cam.append_attribute("y") = camera.y;

	return true;
}


void ModuleRender::SetBackgroundColor(SDL_Color color)
{
	background = color;
}


void ModuleRender::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}


void ModuleRender::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}


// Blit to screen
bool ModuleRender::Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, Uint8 alpha, bool fliped, float pivotX, float pivotY, float speedX, float speedY, double angle, int rotpivot_x, int rotpivot_y)
{
	camera.x = currentCamX;
	camera.y = currentCamY;
	bool ret = true;
	float scale = app->win->GetScale();

	SDL_Rect rect;
	rect.x = (int)(camera.x * speedX) + x * scale;
	rect.y = (int)(camera.y * speedY) + y * scale;

	if (alpha != 0)
	{
		SDL_SetTextureAlphaMod(texture, alpha);
	}
	


	if (section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}


	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point rotPivot;


	if (rotpivot_x != INT_MAX && rotpivot_y != INT_MAX)
	{
		rotPivot.x = rotpivot_x;
		rotPivot.y = rotpivot_y;
		p = &rotPivot;
	}


	if (fliped)
	{
		rect.x = rect.x - rect.w + pivotX;


		if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_HORIZONTAL) != 0)
		{
			LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
			ret = false;
		}


	}
	else
	{
		rect.x = rect.x - pivotX;


		if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0)
		{
			//LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError()); //TODO solve this
			ret = false;
		}
	}

	if (alpha != 0)
	{
		SDL_SetTextureAlphaMod(texture, 255);
	}


	return ret;
}


bool ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera)
{
	camera.x = currentCamX;
	camera.y = currentCamY;
	bool ret = true;
	float scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);


	if (use_camera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}


	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);


	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}


	return ret;
}


bool ModuleRender::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) 
{
	camera.x = currentCamX;
	camera.y = currentCamY;
	bool ret = true;
	float scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;


	if (use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);


	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}


	return ret;
}


bool ModuleRender::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera)
{
	camera.x = currentCamX;
	camera.y = currentCamY;
	bool ret = true;
	float scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;


	for (uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}


	result = SDL_RenderDrawPoints(renderer, points, 360);


	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}


	return ret;
}

void ModuleRender::GetCameraMeasures(int& w, int& h)
{
	w = camera.w;
	h = camera.h;
}

const int ModuleRender::GetCameraX()
{
	return camera.x;
}

const int ModuleRender::GetCameraY()
{
	return camera.y;
}

void ModuleRender::AssignCameraMeasures()
{
	camera.w = app->win->screenSurface->w;
	camera.h = app->win->screenSurface->h;
}

