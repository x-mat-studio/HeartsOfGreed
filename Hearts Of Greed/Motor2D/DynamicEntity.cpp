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

DynamicEntity::DynamicEntity(fMPoint position, int speed, ENTITY_TYPE type, ENTITY_ALIGNEMENT align, Collider* collider, int maxHealth, int currentHealth, int moveRange1, int moveRange2) :

	Entity(position, type, align, collider, maxHealth, currentHealth, true),

	moveRange1(moveRange1),
	moveRange2(moveRange2),
	unitSpeed(speed),

	isMoving(false),

	toMove{ 0,0 },
	framesSinceRequest(0),
	framesToRquest(FRAMES_PER_PATH_REQUEST),
	waitingForPath(false),

	dir(FACE_DIR::SOUTH_EAST)
{
	if (this->align == ENTITY_ALIGNEMENT::ENEMY)
	{
		int randomCounter = rand() % FRAMES_PER_PATH_REQUEST;
		framesToRquest += randomCounter;
	}
}


DynamicEntity::~DynamicEntity()
{
	path.clear();
	closeEntityVector.clear();
	collidingEntityVector.clear();

}


bool DynamicEntity::Move(float dt)
{
	BROFILER_CATEGORY("Move Unit", Profiler::Color::BlanchedAlmond);

	//Speed is resetted to 0 each iteration
	toMove = { 0, 0 };

	// ----------------------------------------------------------------

	fMPoint pathSpeed = { 0,0 };
	fMPoint nextPoint = { 0,0 };

	if (waitingForPath)
	{
		app->pathfinding->RequestPath(this, &path);

		if (!path.empty())
		{
			path.erase(path.begin());
			waitingForPath = false;
		}
	}

	else if (framesSinceRequest >= framesToRquest || path.size() < 2)
	{
		app->pathfinding->RequestPath(this, &path);

		framesSinceRequest = 0;

	}

	if (path.size() > 0)
	{
		app->map->MapToWorldCoords(path[0].x, path[0].y, app->map->data, nextPoint.x, nextPoint.y);

		pathSpeed.create((nextPoint.x - position.x), (nextPoint.y - position.y)).Normalize();
		framesSinceRequest++;
	}

	dir = DetermineDirection(pathSpeed);

	// -----------------------------------------------------------------
	pathSpeed = pathSpeed * unitSpeed;

	toMove += pathSpeed;

	position.x += (toMove.x) * dt;
	position.y += (toMove.y) * dt;

	// ------------------------------------------------------------------

	if (path.size() > 0 && abs(position.x - nextPoint.x) <= 15 && abs(position.y - nextPoint.y) <= 15)
	{
		path.erase(path.begin());
	}

	if (!pathSpeed.IsZero() || waitingForPath)
	{
		return true;
	}


	return false;
}

FACE_DIR DynamicEntity::DetermineDirection(fMPoint faceDir)
{
	FACE_DIR newDir = dir;

	if (faceDir.x > 0)
	{
		if (faceDir.y < -0.3f)
		{
			newDir = FACE_DIR::NORTH_EAST;

		}
		else if (faceDir.y > 0.3f)
		{
			newDir = FACE_DIR::SOUTH_EAST;
		}
		else
		{
			newDir = FACE_DIR::EAST;
		}
	}
	else if (faceDir.x < 0)
	{
		if (faceDir.y < -0.3f)
		{
			newDir = FACE_DIR::NORTH_WEST;
		}
		else if (faceDir.y > 0.3f)
		{
			newDir = FACE_DIR::SOUTH_WEST;

		}
		else
		{
			newDir = FACE_DIR::WEST;
		}
	}


	return newDir;
}

void DynamicEntity::GroupMovement(float dt)
{
	BROFILER_CATEGORY("Group Mov.", Profiler::Color::BlanchedAlmond);


	toMove = { 0,0 };

	// ----------------------------------------------------------------

	app->entityManager->GetEntityNeighbours(&closeEntityVector, &collidingEntityVector, this);

	//---------------------------------------------------------------- 
	fMPoint separationSpeed;

	if (!collidingEntityVector.empty())
	{
		separationSpeed = GetSeparationSpeed(collidingEntityVector, position);
	}
	else
	{
		separationSpeed.x = 0;
		separationSpeed.y = 0;
	}

	// ---------------------------------------------------------------- 

	fMPoint cohesionSpeed = { 0,0 };
	if (!closeEntityVector.empty())
	{
		cohesionSpeed = GetCohesionSpeed(closeEntityVector, position);
	}
	else
	{
		cohesionSpeed.x = 0;
		cohesionSpeed.y = 0;
	}

	//---------------------------------------------------------------- 

	fMPoint alignmentSpeed = { 0,0 };
	if (!closeEntityVector.empty() && (abs(toMove.x) > 0 || abs(toMove.y) > 0))
	{
		alignmentSpeed = GetDirectionSpeed(closeEntityVector);
	}
	else
	{
		alignmentSpeed.x = 0;
		alignmentSpeed.y = 0;
	}

	// ----------------------------------------------------------------- 

	toMove += separationSpeed * 30.f + cohesionSpeed * 7.f + alignmentSpeed * 2.25f;


	position.x += (toMove.x) * dt;
	position.y += (toMove.y) * dt;
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

	colliding_entity_list.clear();

	return separationSpeed;
}

void DynamicEntity::SetSpeed(int speed)
{
	this->unitSpeed = speed;
}

int DynamicEntity::GetSpeed()
{
	return unitSpeed;
}

fMPoint DynamicEntity::GetCohesionSpeed(std::vector<DynamicEntity*>close_entity_list, fMPoint position)
{
	BROFILER_CATEGORY("COHESION SPEED", Profiler::Color::DarkOliveGreen);

	fMPoint MassCenter{ (float)position.x, (float)position.y };

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

	int diameter = 0;

	if (it != nullptr)
		diameter = (it->moveRange1 * 2 + 1);
	else
		return cohesionSpeed;


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


	close_entity_list.clear();
	return cohesionSpeed;
}

fMPoint DynamicEntity::GetDirectionSpeed(std::vector<DynamicEntity*>close_entity_list)
{
	BROFILER_CATEGORY("DIRECTION SPEED", Profiler::Color::Magenta);

	fMPoint alignmentSpeed{ 0, 0 };

	DynamicEntity* it;

	for (int i = 0; i < close_entity_list.size(); i++)
	{
		it = close_entity_list[i];

		alignmentSpeed.x += it->unitSpeed;
		alignmentSpeed.y += it->unitSpeed;
	}

	alignmentSpeed.x = alignmentSpeed.x / close_entity_list.size();
	alignmentSpeed.y = alignmentSpeed.y / close_entity_list.size();

	float norm = sqrt(alignmentSpeed.x * alignmentSpeed.x + alignmentSpeed.y * alignmentSpeed.y);

	if (norm != 0)
	{
		alignmentSpeed = alignmentSpeed / norm;
	}

	close_entity_list.clear();

	return alignmentSpeed;
}

bool DynamicEntity::GeneratePath(float x, float y, int lvl)
{
	BROFILER_CATEGORY("Generate Path", Profiler::Color::Aquamarine);

	iMPoint goal = { 0,0 };

	origin = app->map->WorldToMap(round(position.x), round(position.y));
	goal = app->map->WorldToMap(x, y);

	if (app->pathfinding->GetDestination(this) != goal || (!this->path.empty() && this->path.back() == goal))
		if (app->pathfinding->GeneratePath(origin, goal, lvl, this) != PATH_TYPE::NO_TYPE)
		{
			waitingForPath = true;

			path.clear();

			return true;
		}

	return false;
}

void DynamicEntity::DebugDraw()
{
	if (!app->debugMode)
	{
		return;
	}

	//Position --------------------------------------
	app->render->DrawQuad({ (int)position.x, (int)position.y, 2,2 }, 255, 0, 0);

	fMPoint nextPoint = { 0,0 };
	origin = app->map->WorldToMap(round(position.x), round(position.y));
	origin = app->map->MapToWorld(origin.x, origin.y);

	app->render->DrawQuad({ (int)origin.x, (int)origin.y, 10,10 }, 255, 255, 255, 125);


	// Debug pathfinding ------------------------------

	std::vector<iMPoint>* path = &this->path;
	for (std::vector<iMPoint>::iterator it = path->begin(); it != path->end(); ++it)
	{
		iMPoint pos = app->map->MapToWorld(it->x - 1, it->y);
		app->render->Blit(app->entityManager->debugPathTexture, pos.x, pos.y);
	}
}

void DynamicEntity::DestroyPath()
{
	app->pathfinding->DeletePath(this);
}

void DynamicEntity::Draw(float dt)
{}
