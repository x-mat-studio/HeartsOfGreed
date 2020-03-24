#include "DynamicEntity.h"
#include "Pathfinding.h"
#include "Brofiler/Brofiler/Brofiler.h"
#include "Map.h"
#include "EntityManager.h"
#include "TestScene.h"
#include "Input.h"
#include "Render.h"

DynamicEntity::DynamicEntity(iMPoint position, ENTITY_TYPE type, Collider* collider, int moveRange1, int moveRange2) :
	moveRange1(moveRange1), moveRange2(moveRange2), speed(0, 0), pathToFollow(0), isMoving(false), Entity(position, type, collider, true) {}

DynamicEntity::~DynamicEntity()
{}

bool DynamicEntity::Move()
{
	BROFILER_CATEGORY("UpdateTest_1", Profiler::Color::BlanchedAlmond);

	//Speed is resetted to 0 each iteration
	speed = { 0, 0 };
	app->map->WorldToMapCoords(position.x, position.y, app->map->data, origin.x, origin.y);


	// ----------------------------------------------------------------

	fMPoint pathSpeed{ 0,0 };
	fMPoint pathSpeed2{ 0,0 };
	fMPoint nextPoint;
	if (path.size() > 0)
	{
		app->map->MapToWorldCoords(path[0].x, path[0].y, app->map->data, nextPoint.x, nextPoint.y);

		pathSpeed2.create((nextPoint.x - position.x), (nextPoint.y - position.y));

		pathSpeed2.Normalize();
		
		if ((origin.x == path[0].x && origin.y == path[0].y) )
		{
			path.erase(path.begin());
		}
	}

	std::list<Entity*>::iterator neighbours_it;

	// ----------------------------------------------------------------

	app->entityManager->GetEntityNeighbours(&close_entity_list, &colliding_entity_list, this);

	//---------------------------------------------------------------- 
	fMPoint separationSpeed;

	if (!colliding_entity_list.empty())
	{
		separationSpeed = GetSeparationSpeed(colliding_entity_list, position);
	}
	else
	{
		separationSpeed.x = 0;
		separationSpeed.y = 0;
	}


	// ---------------------------------------------------------------- 

	fMPoint cohesionSpeed = { 0,0 };
	if (!close_entity_list.empty())
	{
		cohesionSpeed = GetCohesionSpeed(close_entity_list, position);
	}
	else
	{
		cohesionSpeed.x = 0;
		cohesionSpeed.y = 0;
	}

	//---------------------------------------------------------------- 

	fMPoint alignmentSpeed = { 0,0 };
	if (!close_entity_list.empty() && (abs(pathSpeed.x) > 0 || abs(pathSpeed.y) > 0))
	{
		alignmentSpeed = GetDirectionSpeed(close_entity_list);
	}
	else
	{
		alignmentSpeed.x = 0;
		alignmentSpeed.y = 0;
	}


	// ---------------------------------------------------------------- 

	speed += pathSpeed2 * 1.5f + separationSpeed * 1 + cohesionSpeed * 0.5f + alignmentSpeed * 0.1f;

	// ------------------------------------------------------------------

	position.y += speed.y;
	position.x += speed.x;

	if (!pathSpeed.IsZero())
		return true;
	else
		return false;

}

fMPoint DynamicEntity::GetSeparationSpeed(std::list<DynamicEntity*>colliding_entity_list, iMPoint position)
{
	BROFILER_CATEGORY("SEPARATION SPEED", Profiler::Color::Aquamarine);

	fMPoint separationSpeed = { 0,0 };

	if (colliding_entity_list.size() == 0)
		return separationSpeed;

	std::list<DynamicEntity*>::iterator entities_list;
	DynamicEntity* it;
	for (entities_list = colliding_entity_list.begin(); entities_list != colliding_entity_list.end(); ++entities_list)
	{
		it = *entities_list;

		separationSpeed.x += (position.x - it->GetPosition().x);
		separationSpeed.y += (position.y - it->GetPosition().y);

	}

	separationSpeed.x /= colliding_entity_list.size();
	separationSpeed.y /= colliding_entity_list.size();

	float spdNorm = sqrtf(separationSpeed.x * separationSpeed.x + separationSpeed.y * separationSpeed.y);

	if (spdNorm == 0)
		separationSpeed = { 0,0 };
	else
	{
		separationSpeed.x /= spdNorm;
		separationSpeed.y /= spdNorm;
	}


	return separationSpeed;
}

fMPoint DynamicEntity::GetCohesionSpeed(std::list<DynamicEntity*>close_entity_list, iMPoint position)
{
	BROFILER_CATEGORY("COHESION SPEED", Profiler::Color::DarkOliveGreen);

	fMPoint MassCenter
	{
		(float)position.x, (float)position.y
	};
	std::list<DynamicEntity*>::iterator neighbours_it;
	DynamicEntity* it = nullptr;

	for (neighbours_it = close_entity_list.begin(); neighbours_it != close_entity_list.end(); ++neighbours_it)
	{
		it = *neighbours_it;
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

fMPoint DynamicEntity::GetDirectionSpeed(std::list<DynamicEntity*>close_entity_list)
{
	BROFILER_CATEGORY("DIRECTION SPEED", Profiler::Color::Magenta);


	fMPoint alignmentSpeed{
	0, 0
	};

	std::list<DynamicEntity*>::iterator neighbours_it;
	DynamicEntity* it;

	for (neighbours_it = close_entity_list.begin(); neighbours_it != close_entity_list.end(); ++neighbours_it) {

		it = *neighbours_it;

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
	int X, Y = 0;

	app->map->WorldToMapCoords(position.x, position.y, app->map->data, origin.x, origin.y);
	app->map->WorldToMapCoords(x - app->map->data.tileWidth * 0.5f, y, app->map->data, X, Y);

	if (app->pathfinding->CreatePath(origin, { X,Y }) == 0)
	{
		path.clear();
		app->pathfinding->SavePath(&path);
		pathToFollow = 1;
		return true;
	}

	return false;
}

void DynamicEntity::DebugDraw()
{
	// Debug pathfinding ------------------------------

	if (!app->debugMode)
		return;

	SDL_Texture* debug_tex = app->entityManager->debugPathTexture;

	int x, y;
	app->input->GetMousePosition(x, y);
	iMPoint p = app->map->MapToWorld(x, y);

	app->render->Blit(debug_tex, p.x, p.y);

	std::vector<iMPoint>* path = &this->path;


	for (std::vector<iMPoint>::iterator it = path->begin(); it != path->end(); ++it)
	{
		iMPoint pos = app->map->MapToWorld(it->x, it->y);
		app->render->Blit(debug_tex, pos.x, pos.y);
	}
}