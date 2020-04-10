#pragma once

#ifndef __DYNAMICENTITY_H__
#define __DYNAMICENTITY_H__

#include "Entity.h"

#define FRAMES_PER_PATHFINDING 30

enum class FACE_DIR : int
{
	NONE = -1,

	NORTH,
	NORTH_EAST,
	EAST,
	SOUTH_EAST,
	SOUTH,
	SOUTH_WEST,
	WEST,
	NORTH_WEST
};

class DynamicEntity : public Entity
{
public:

	DynamicEntity(fMPoint position, iMPoint speed, ENTITY_TYPE type, ENTITY_ALIGNEMENT align, Collider* collider, int moveRange1, int moveRange2);

	~DynamicEntity();

	//Returns true while moving, returns false when stops
	bool Move(float dt);
	void GroupMovement(float dt);

	bool GeneratePath(float x, float y, int lvl);
	void DebugDraw();
	virtual void OnCollision(Collider* collider) {};
	void Draw(float dt);

	FACE_DIR DetermineDirection(fMPoint dir);

public:
	bool isMoving;

	//used for moving close entities to have an smooth group movement
	int moveRange1;
	int moveRange2;

	iMPoint unitSpeed;

	FACE_DIR dir;

protected:

	iMPoint origin, mouse;
	Animation* current_animation;
	std::vector <iMPoint> path;
	std::vector<DynamicEntity*> closeEntityList;
	std::vector<DynamicEntity*> collidingEntityList;

private:
	fMPoint toMove;


private:
	fMPoint DynamicEntity::GetDirectionSpeed(std::vector<DynamicEntity*>closeEntityList);
	fMPoint DynamicEntity::GetCohesionSpeed(std::vector<DynamicEntity*>closeEntityList, fMPoint position);
	fMPoint DynamicEntity::GetSeparationSpeed(std::vector<DynamicEntity*>collidingEntityList, fMPoint position);
	SDL_Rect GetAnimationRect(float dt);

};

#endif//__DYNAMICENTITY_H__