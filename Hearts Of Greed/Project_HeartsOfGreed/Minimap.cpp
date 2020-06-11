#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Minimap.h"
#include "Map.h"
#include "Textures.h"
#include "Window.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "UI_Group.h"
#include "UIManager.h"
#include "UIFactory.h"
#include "UI.h"
#include "Player.h"
#include "FoWManager.h"
#include "Brofiler/Brofiler/Brofiler.h"
#include "TestScene.h"
#include "Entity.h"

MinimapIcon::MinimapIcon(fMPoint* worldPos, MINIMAP_ICONS type, fMPoint& offSet, Entity* nparent) :
	toDelete(false),
	type(type),
	offSet(offSet),
	minimapPos(worldPos),
	parent(nparent),
	active(true)
{}


MinimapIcon::~MinimapIcon()
{
	minimapPos = nullptr;
	parent = nullptr;
}

void MinimapIcon::Draw(SDL_Rect sourceRect)
{
	if (minimapPos != nullptr)
	{
		iMPoint newpos = app->minimap->WorldToMinimap(minimapPos->x, minimapPos->y);
		float uiscale = app->win->GetUIScale();
		app->render->Blit(app->uiManager->GetAtlasTexture(), (newpos.x - (sourceRect.w * 0.5f)) / uiscale, (newpos.y - (sourceRect.h * 0.5f)) / uiscale, &sourceRect, false, false);
	}
}

void MinimapIcon::SetActiveState(bool isActive)
{
	active = isActive;
}

bool MinimapIcon::IsActive() const
{
	return active;
}


Minimap::Minimap() :
	minimapLoaded(false),
	minimapFoWNeedsUpdate(false),
	miniFrame({ 509, 706, 238, 125 }),
	positionFrame({ 0,0,0,0 }),
	camRect({ 0,0,0,0 }),
	minimapTexture(nullptr),
	minimapFrame(nullptr),
	minimapHeight(0),
	width(0),
	position({ 0,0 }),
	height(0),
	minimapWidth(0),
	minimapScaleRelation(0)
{
	name = "minimap";
}


Minimap::~Minimap()
{
	minimapIcons.clear();

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
	app->eventManager->EventRegister(EVENT_ENUM::FULLSCREEN_REGAIN_FOCUS, this);

	return ret;

}


// Called each loop iteration
bool Minimap::PreUpdate(float dt)
{
	CheckListener(this);
	if (minimapLoaded)
	{
		//deletes all the icons that request to do so
		for (int i = 0; i < minimapIcons.size(); i++)
		{
			if (minimapIcons[i]->toDelete == true)
			{
				delete minimapIcons[i];
				minimapIcons[i] = nullptr;
				minimapIcons.erase(minimapIcons.begin() + i);
				i--;
			}

		}

		float UIscale = app->win->GetUIScale();
		/*UI* element = app->uiManager->FindUIByName("minimapBackground");
		position.x = element->worldPosition.x * UIscale + 5;
		position.y = element->worldPosition.y * UIscale + 7;*/
	}
	return true;
}


// Called each loop iteration
bool Minimap::Update(float dt)
{
	BROFILER_CATEGORY("Minimap Update", Profiler::Color::DarkBlue);


	CheckListener(this);

	if (minimapLoaded == true)
	{
		iMPoint mousePos = app->input->GetMousePosScreen();
		int w;
		int h;

		app->render->GetCameraMeasures(w, h);

		float scale = app->win->GetScale();
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_STATE::KEY_DOWN || app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_STATE::KEY_REPEAT)
		{
			if (ClickingOnMinimap(mousePos.x, mousePos.y) == true && app->player->doingAction == false)
			{
				//camera TP
				iMPoint newCamPos = ScreenToMinimapToWorld(mousePos.x, mousePos.y);
				app->render->currentCamX = -((newCamPos.x * scale) - (w * 0.5f));
				app->render->currentCamY = -((newCamPos.y * scale) - (h * 0.5f));
				app->testScene->SetCamEasingState(false);//TODO make this be an event to be able to erase the testscene.h include
			}
		}
	}
	return true;
}


// Called each loop iteration
bool Minimap::PostUpdate(float dt)
{
	bool ret = true;
	BROFILER_CATEGORY("Minimap PostUpdate", Profiler::Color::DarkSlateBlue);

	if (minimapLoaded == true)
	{
		if (minimapFoWNeedsUpdate == true)
		{
			UpdateMinimapFoW();
			minimapFoWNeedsUpdate = false;
		}
		//app->render->MinimapBlit(minimapTexture, position.x, position.y, NULL, 1.0);
		//FoW Draw


		//Chenges the texture tint if is nighttime

		float t = app->testScene->GetNightRectAlpha();
		if (t != 0)
		{
			t *= 0.01;

			float r = ((1 - t) * 255) + (t * 96);
			float g = ((1 - t) * 255) + (t * 63);
			float b = ((1 - t) * 255) + (t * 148);

			SDL_SetTextureColorMod(minimapTexture,r, g, b);

		}


		app->render->MinimapBlit(minimapTexture, position.x, position.y, NULL, 1.0);

		SDL_SetTextureColorMod(minimapTexture, 255, 255, 255);

		positionFrame = { -14, 496, 465, 240 };
		SDL_RenderCopy(app->render->renderer, app->uiManager->GetAtlasTexture(), &miniFrame, &positionFrame);


		//Draw icons
		SDL_Rect iconRect = { 0,0,0,0 };
		for (int i = 0; i < minimapIcons.size(); i++)
		{
			if (minimapIcons[i]->IsActive() == true)
			{
				bool visible = true; //todo kind of redundant visible code, take a look at that for the gold
				if (minimapIcons[i]->parent != nullptr && minimapIcons[i]->parent->visionEntity != nullptr)
				{
					if (minimapIcons[i]->parent->visionEntity->isVisible == false)
					{
						visible = false;
					}
				}

				if (visible == true)
				{
					switch (minimapIcons[i]->type)
					{
					case MINIMAP_ICONS::BASE:
						iconRect = { 12, 504, 4, 4 };
						break;
					case MINIMAP_ICONS::TURRET:
						iconRect = { 20, 504, 4, 4 };
						break;
					case MINIMAP_ICONS::ENEMY_TURRET:
						iconRect = { 16, 504, 4, 4 };
						break;
					case MINIMAP_ICONS::HERO:
						iconRect = { 8, 504, 4, 4 };
						break;
					case MINIMAP_ICONS::ENEMY:
						iconRect = { 0, 504, 4, 4 };
						break;
					case MINIMAP_ICONS::ENEMY_BASE:
						iconRect = { 4, 504, 4, 4 };
						break;
					case MINIMAP_ICONS::QUEST:
						iconRect = { 24, 504, 4, 4 };//TODO change this
						break;
					case MINIMAP_ICONS::NONE:
						iconRect = { 0, 0, 0, 0 };
						break;
					}

					minimapIcons[i]->Draw(iconRect);
				}
			}
		}




		//camera rect representation
		SDL_Rect cam;

		float scale = app->win->GetScale();
		int w;
		int h;
		int x = -(app->render->GetCameraX()) / scale;
		int y = -(app->render->GetCameraY()) / scale;
		app->render->GetCameraMeasures(w, h);

		iMPoint camXY = WorldToMinimap(x, y);
		cam.x = camXY.x;
		cam.y = camXY.y;
		cam.w = w * minimapScaleRelation / scale;
		cam.h = h * minimapScaleRelation / scale;

		//rectangle trim
		//right
		if (cam.x + cam.w > position.x + width)
		{
			cam.w = MAX(position.x + width - cam.x, 0);
		}
		//bottom
		if (cam.y + cam.h > position.y + height)
		{
			cam.h = MAX(position.y + height - cam.y, 0);
		}
		//left
		if (cam.x < position.x)
		{
			cam.w -= position.x - cam.x;
			cam.w = MAX(cam.w, 0);
			cam.x += position.x - cam.x;
		}
		if (cam.y < position.y)
		{
			cam.h -= position.y - cam.y;
			cam.h = MAX(cam.h, 0);
			cam.y += position.y - cam.y;
		}

		cam.x = MAX(cam.x, position.x);
		cam.x = MIN(cam.x, position.x + width);
		cam.y = MAX(cam.y, position.y);
		cam.y = MIN(cam.y, position.y + height);

		app->render->DrawQuad(cam, 255, 255, 255, 255, false, false);
	}


	return ret;
}


// Called before quitting
bool Minimap::CleanUp()
{
	int numElements = minimapIcons.size();

	for (int i = 0; i < numElements; i++)
	{
		RELEASE(minimapIcons[i]);
		minimapIcons[i] = nullptr;
	}

	minimapIcons.clear();

	app->eventManager->EventUnRegister(EVENT_ENUM::FULLSCREEN_REGAIN_FOCUS, this);

	app->tex->UnLoad(minimapTexture);
	minimapLoaded = false;
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
	case EVENT_ENUM::FULLSCREEN_REGAIN_FOCUS:
		UpdateMinimapFoW();
		break;
	}
}

void Minimap::CreateMinimapText()
{
	fMPoint auxCam;
	auxCam.x = app->render->currentCamX;
	auxCam.y = app->render->currentCamY;
	app->render->currentCamX = 0.0f;
	app->render->currentCamY = 0.0f;

	app->map->DrawMinimap();
	app->entityManager->DrawOnlyStaticBuildings();
	//app->fowManager->DrawFoWMinimap(); //detalis & bases are not visible in the minimap so we can not draw the FoW
	app->render->currentCamX = auxCam.x;
	app->render->currentCamY = auxCam.y;
}

void Minimap::LoadMinimap()
{

	BROFILER_CATEGORY("Load Minimap", Profiler::Color::Red);

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
	minimapFoWNeedsUpdate = true;

}

void Minimap::UpdateMinimapFoW()
{
	if (minimapLoaded == true)
	{
		BROFILER_CATEGORY("Update Minimap FoW", Profiler::Color::LightCoral);
		app->tex->UnLoad(minimapTexture);
		minimapTexture = nullptr;

		uint windowWidth, windowHeight;
		app->win->GetWindowSize(windowWidth, windowHeight);

		minimapTexture = SDL_CreateTexture(app->render->renderer, SDL_GetWindowPixelFormat(app->win->window), SDL_TEXTUREACCESS_TARGET, 1.05f * width, 1.05f * height);

		SDL_SetRenderTarget(app->render->renderer, minimapTexture);
		CreateMinimapText();
		SDL_SetRenderTarget(app->render->renderer, NULL);
	}
}
void Minimap::MinimapFoWNeedsUpdate()
{
	if (minimapFoWNeedsUpdate == false)
		minimapFoWNeedsUpdate = true;
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

MinimapIcon* Minimap::CreateIcon(fMPoint* worldPos, MINIMAP_ICONS type, fMPoint& offset, Entity* parent)
{
	MinimapIcon* icon = nullptr;

	icon = new MinimapIcon(worldPos, type, offset, parent);

	if (icon != nullptr)
	{
		minimapIcons.push_back(icon);
	}

	return icon;
}

void Minimap::SetAllIconsActiveState(bool areActive)
{
	for (int i = 0; i < minimapIcons.size(); i++)
	{
		minimapIcons[i]->SetActiveState(areActive);
	}
}
