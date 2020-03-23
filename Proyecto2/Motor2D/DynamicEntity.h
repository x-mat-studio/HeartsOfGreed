#pragma once

#ifndef __DYNAMICENTITY_H__
#define __DYNAMICENTITY_H__

#include "Entity.h"
#include <list>

class DynamicEntity : public Entity
{
public:

	DynamicEntity(iMPoint position, ENTITY_TYPE type, Collider* collider, int moveRange1, int moveRange2);

	~DynamicEntity();

	//Returns true while moving, returns false when stops
	bool Move();

	bool GeneratePath(int x, int y);
	void DebugDraw();

public:
	int pathToFollow;
	bool isMoving;
	//used for moving close entities to have an smooth group movement
	int moveRange1;
	int moveRange2;

	fMPoint speed;

protected:

	iMPoint origin, mouse;
	std::vector <iMPoint> path;
	std::list<DynamicEntity*> close_entity_list;
	std::list<DynamicEntity*> colliding_entity_list;

private:
	fMPoint DynamicEntity::GetDirectionSpeed(std::list<DynamicEntity*>close_entity_list);
	fMPoint DynamicEntity::GetCohesionSpeed(std::list<DynamicEntity*>close_entity_list, iMPoint position);
	fMPoint DynamicEntity::GetSeparationSpeed(std::list<DynamicEntity*>colliding_entity_list, iMPoint position);

};

#endif//__DYNAMICENTITY_H__