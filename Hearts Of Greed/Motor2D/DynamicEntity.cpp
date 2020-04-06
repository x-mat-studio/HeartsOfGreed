#include "DynamicEntity.h"
#include "Pathfinding.h"
#include "Brofiler/Brofiler/Brofiler.h"
#include "Map.h"
#include "EntityManager.h"
#include "TestScene.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"
#include "App.h"

DynamicEntity::DynamicEntity(fMPoint position, ENTITY_TYPE type, ENTITY_ALIGNEMENT align, Collider* collider, int moveRange1, int moveRange2) :
	
	Entity(position, type, align, collider, true),

	moveRange1(moveRange1), 
	moveRange2(moveRange2), 
	speed(0, 0), 
	isMoving(false), 
	current_animation(nullptr)
{}

DynamicEntity::~DynamicEntity()
{}

bool DynamicEntity::Move(float dt)
{
	BROFILER_CATEGORY("Move Unit", Profiler::Color::BlanchedAlmond);

	//Speed is resetted to 0 each iteration
	speed = { 0, 0 };

	// ----------------------------------------------------------------

	fMPoint pathSpeed;
	pathSpeed.create(0, 0);
	fMPoint nextPoint;

	if (path.size() > 0)
	{
		app->map->MapToWorldCoords(path[0].x, path[0].y, app->map->data, nextPoint.x, nextPoint.y);

		pathSpeed.create((nextPoint.x - position.x), (nextPoint.y - position.y)).Normalize();
	}

	// ----------------------------------------------------------------

	app->entityManager->GetEntityNeighbours(&closeEntityList, &collidingEntityList, this);

	//---------------------------------------------------------------- 
	fMPoint separationSpeed;

	if (!collidingEntityList.empty())
	{
		separationSpeed = GetSeparationSpeed(collidingEntityList, position);
	}
	else
	{
		separationSpeed.x = 0;
		separationSpeed.y = 0;
	}

	// ---------------------------------------------------------------- 

	fMPoint cohesionSpeed = { 0,0 };
	if (!closeEntityList.empty())
	{
		cohesionSpeed = GetCohesionSpeed(closeEntityList, position);
	}
	else
	{
		cohesionSpeed.x = 0;
		cohesionSpeed.y = 0;
	}

	//---------------------------------------------------------------- 

	fMPoint alignmentSpeed = { 0,0 };
	if (!closeEntityList.empty() && (abs(pathSpeed.x) > 0 || abs(pathSpeed.y) > 0))
	{
		alignmentSpeed = GetDirectionSpeed(closeEntityList);
	}
	else
	{
		alignmentSpeed.x = 0;
		alignmentSpeed.y = 0;
	}

	// ----------------------------------------------------------------- 

	speed += pathSpeed + separationSpeed * 1 + cohesionSpeed * 0.5f + alignmentSpeed * 0.1f;

	// ------------------------------------------------------------------

	position.x += (speed.x) * dt;
	position.y += (speed.y) * dt;

	if (path.size() > 0 && abs(position.x - nextPoint.x) <= 5  && abs(position.y - nextPoint.y) <= 5)
	{
		path.erase(path.begin());
	}

	if (pathSpeed.IsZero())
	{
		return true;
	}
	else
	{
		return false;
	}

}

fMPoint DynamicEntity::GetSeparationSpeed(std::vector<DynamicEntity*>colliding_entity_list, fMPoint position)
{
	BROFILER_CATEGORY("SEPARATION SPEED", Profiler::Color::Aquamarine);

	fMPoint separationSpeed = { 0,0 };

	if (colliding_entity_list.size() == 0)
	{
		return separationSpeed;
	}

	DynamicEntity* it;

	for (int i = 0; i < colliding_entity_list.size(); i++)
	{
		it = colliding_entity_list[i];

		separationSpeed.x += (position.x - it->GetPosition().x);
		separationSpeed.y += (position.y - it->GetPosition().y);
	}

	separationSpeed.x /= colliding_entity_list.size();
	separationSpeed.y /= colliding_entity_list.size();

	float spdNorm = sqrtf(separationSpeed.x * separationSpeed.x + separationSpeed.y * separationSpeed.y);

	if (spdNorm == 0)
	{
		separationSpeed = { 0,0 };
	}
	else
	{
		separationSpeed.x /= spdNorm;
		separationSpeed.y /= spdNorm;
	}


	return separationSpeed;
}

fMPoint DynamicEntity::GetCohesionSpeed(std::vector<DynamicEntity*>close_entity_list, fMPoint position)
{
	BROFILER_CATEGORY("COHESION SPEED", Profiler::Color::DarkOliveGreen);

	fMPoint MassCenter{(float)position.x, (float)position.y};

	DynamicEntity* it = nullptr;

	for (int i = 0; i < close_entity_list.size(); i++)
	{
		it = close_entity_list[i];

		MassCenter.x += it->GetPosition().x;
		MassCenter.y += it->GetPosition().y;
	}

	MassCenter.x = MassCenter.x / (close_entity_list.size() + 1);
	MassCenter.y = MassCenter.y / (close_entity_list.size() + 1);

	fMPoint cohesionSpeed = { MassCenter.x - position.x, MassCenter.y - position.y };

	float norm = sqrt(cohesionSpeed.x * cohesionSpeed.x + cohesionSpeed.y * cohesionSpeed.y);

	int diameter = (it->moveRange1 * 2 + 1);

	if (cohesionSpeed.x < diameter && cohesionSpeed.x > -diameter)
	{
		cohesionSpeed.x = 0;
	}
	else
	{
		cohesionSpeed.x = cohesionSpeed.x / norm;
	}
	if (cohesionSpeed.y < diameter && cohesionSpeed.y > -diameter)
	{
		cohesionSpeed.y = 0;
	}
	else
	{
		cohesionSpeed.y = cohesionSpeed.y / norm;
	}

	return cohesionSpeed;
}

fMPoint DynamicEntity::GetDirectionSpeed(std::vector<DynamicEntity*>close_entity_list)
{
	BROFILER_CATEGORY("DIRECTION SPEED", Profiler::Color::Magenta);

	fMPoint alignmentSpeed{0, 0};

	DynamicEntity* it;

	for (int i = 0; i < close_entity_list.size(); i++)
	{
		it = close_entity_list[i];

		alignmentSpeed += it->speed;
	}

	alignmentSpeed.x = alignmentSpeed.x / close_entity_list.size();
	alignmentSpeed.y = alignmentSpeed.y / close_entity_list.size();

	float norm = sqrt(alignmentSpeed.x * alignmentSpeed.x + alignmentSpeed.y * alignmentSpeed.y);

	if (norm != 0)
	{
		alignmentSpeed = alignmentSpeed / norm;
	}

	return alignmentSpeed;
}

bool DynamicEntity::GeneratePath(int x, int y)
{
	iMPoint goal = { 0,0 };

	app->map->WorldToMapCoords(round(position.x), round(position.y), app->map->data, origin.x, origin.y);
	goal = app->map->WorldToMap(x , y );

	if (app->pathfinding->CreatePath(origin, goal) == 0)
	{
		path.clear();
		app->pathfinding->SavePath(&path);
		path.erase(path.begin());
		return true;
	}

	return false;
}

void DynamicEntity::DebugDraw()
{
	//Position --------------------------------------
	app->render->DrawQuad({ (int)position.x, (int)position.y, 2,2 }, 255, 0, 0);

	fMPoint nextPoint = { 0,0 };
	app->map->WorldToMapCoords(round(position.x), round(position.y), app->map->data, origin.x, origin.y);
	origin = app->map->MapToWorld(origin.x, origin.y);

	app->render->DrawQuad({ (int)origin.x, (int)origin.y, 10,10 }, 255, 255, 255, 125);


	// Debug pathfinding ------------------------------

	if (!app->debugMode)
	{
		return;
	}

	SDL_Texture* debugTex = app->entityManager->debugPathTexture;

	std::vector<iMPoint>* path = &this->path;
	for (std::vector<iMPoint>::iterator it = path->begin(); it != path->end(); ++it)
	{
		iMPoint pos = app->map->MapToWorld(it->x-1, it->y);
		app->render->Blit(debugTex, pos.x, pos.y);
	}
}

SDL_Rect DynamicEntity::GetAnimationRect(float dt)
{
	if (current_animation == NULL)
	{
		SDL_Rect rec;
		rec.x = 0;
		rec.y = 0;
		rec.w = 0;
		rec.h = 0;
		return rec;
	}
	else
	{
		return current_animation->GetCurrentFrameBox(dt);
	}
}

void DynamicEntity::Draw(float dt)
{}
