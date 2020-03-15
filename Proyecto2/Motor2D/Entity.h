#pragma once

#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "vector"
#include "SDL/include/SDL_rect.h"
#include "Collision.h"

struct Collider;
struct SDL_Texture;

enum class ENTITY_TYPE
{
	UNKNOWN,
	PARTICLE,
	EMITER,
	PARTICLE_SYSTEM,
	HERO_MELEE,
	HERO_RANGED,
	HERO_GATHERER,
	ENEMY,
	BASE,
	BLDG_TURRET,
	BLDG_UPGRADE_CENTER,
	BLDG_BASE,
	BLDG_BARRICADE,

	MAX_TYPE
};

class Entity
{
public:

	Entity();
	Entity(SDL_Point position, ENTITY_TYPE type, SDL_Texture* texture);
	Entity(SDL_Point position, ENTITY_TYPE type, SDL_Texture* texture, SDL_Rect collRect, COLLIDER_TYPE collType, Module* callback);
	virtual ~Entity();

	virtual bool Start();
	virtual bool PreUpdate(float dt);
	virtual bool Update(float dt);
	virtual bool PostUpdate(float dt);

	virtual void OnCollision(Collider* collider);
	virtual Collider* GetCollider() const;

	SDL_Point GetPosition();
	ENTITY_TYPE GetType();

	void SetToDelete(bool toDelete);

protected:
	virtual void Draw(float dt);

public:
	bool started;
	bool toDelete;
	int vectorPosition;

protected:
	SDL_Point position;

	ENTITY_TYPE type;

	SDL_Texture* texture;

	Collider* collider;

};

#endif//__ENTITY_H__