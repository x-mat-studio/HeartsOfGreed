#ifndef __Collision_H__
#define __Collision_H__

#define MAX_COLLIDERS 200

#include "SDL/include/SDL.h"
#include "Module.h"

class Entity;

enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,

	COLLIDER_FLOOR,
	COLLIDER_PLAYER,
	COLLIDER_ENEMY,
	COLLIDER_DEATH,
	COLLIDER_MAX,
};


struct Collider
{
	SDL_Rect		rect;
	bool			to_delete = false;
	COLLIDER_TYPE	type;
	Entity*			callback = nullptr;


	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, Entity* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
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

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, Entity* callback = nullptr);
	void AddColliderEntity(Collider* collider);
	void DebugDraw();

private:

	bool		debug = false;
	Collider* colliders[MAX_COLLIDERS];
	bool		matrix[COLLIDER_MAX][COLLIDER_MAX];

};

#endif // __Collision_H__
