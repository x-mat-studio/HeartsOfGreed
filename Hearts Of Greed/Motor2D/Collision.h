#ifndef __Collision_H__
#define __Collision_H__

#define MAX_COLLIDERS 3500

#include "SDL/include/SDL.h"
#include "Module.h"

class Entity;

enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,

	COLLIDER_WALL,
	COLLIDER_HERO,
	COLLIDER_ENEMY,
	COLLIDER_DEATH,
	COLLIDER_VISIBILITY,

	COLLIDER_BASE_ALERT,
	COLLIDER_RECLUIT_IA,

	COLLIDER_MAX,
};


struct Collider
{
	SDL_Rect		rect;
	bool			to_delete = false;
	COLLIDER_TYPE	type;
	Module* callback = nullptr;
	Entity* thisEntity = nullptr;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, Module* callback = nullptr, Entity* thisEntity = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback),
		thisEntity(thisEntity)
	{}

	Collider(Collider* collider, Entity* thisEntity = nullptr) :
		rect(collider->rect),
		type(collider->type),
		callback(collider->callback),
		thisEntity(thisEntity)

	{}


	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}


	void setType(COLLIDER_TYPE type1)
	{
		type = type1;
	}


	bool CheckCollision(const SDL_Rect& r) const;
};


class ModuleCollision : public Module
{

public:

	ModuleCollision();
	~ModuleCollision();

	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, Module* callback = nullptr);

	void AddColliderEntity(Collider* collider);
	void DebugDraw();

private:

	bool		debug = false;
	std::vector <Collider*> colliders;
	bool		matrix[COLLIDER_MAX][COLLIDER_MAX];

};

#endif // __Collision_H__
