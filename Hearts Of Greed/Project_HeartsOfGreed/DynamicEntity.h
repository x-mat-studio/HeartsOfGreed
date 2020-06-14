#pragma once

#ifndef __DYNAMICENTITY_H__
#define __DYNAMICENTITY_H__

#include "Entity.h"

#define FRAMES_PER_PATHFINDING 20
#define FRAMES_PER_PATH_REQUEST 30

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

	DynamicEntity(fMPoint position, int speed, ENTITY_TYPE type, ENTITY_ALIGNEMENT align, Collider* collider, int maxHealth, int currentHealth, int moveRange1, int moveRange2);

	~DynamicEntity();

	//Returns true while moving, returns false when stops
	bool Move(float dt);
	void GroupMovement(float dt);

	bool GeneratePath(float x, float y, int lvl);
	void DebugDraw();
	virtual void OnCollision(Collider* collider) {};
	void Draw(float dt);
	void DestroyPath();

	FACE_DIR DetermineDirection(fMPoint dir);

	bool IsMoving() const;

private:
	fMPoint GetDirectionSpeed(std::vector<DynamicEntity*>closeEntityList);
	fMPoint GetCohesionSpeed(std::vector<DynamicEntity*>closeEntityList, fMPoint position);
	fMPoint GetSeparationSpeed(std::vector<DynamicEntity*>collidingEntityList, fMPoint position);

public:
	bool isMoving;
	bool waitingForPath;

	//used for moving close entities to have an smooth group movement
	int moveRange1;
	int moveRange2;

	int unitSpeed;

	FACE_DIR dir;

	int framesSinceRequest;
	int framesToRquest;

	void SetSpeed(int speed);
	int GetSpeed();

protected:

	iMPoint origin, destination;
	std::vector <iMPoint> path;
	std::vector<DynamicEntity*> closeEntityVector;
	std::vector<DynamicEntity*> collidingEntityVector;

private:
	fMPoint toMove;



};

#endif//__DYNAMICENTITY_H__