#pragma once

#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "SDL/include/SDL_rect.h"
#include "Collision.h"
#include "Animation.h"

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
	BUILDING,
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
	Entity(SDL_Point position, ENTITY_TYPE type);
	Entity(SDL_Point position, ENTITY_TYPE type, Collider* collider);
	virtual ~Entity();

	virtual bool Start(SDL_Texture* texture);
	virtual bool PreUpdate(float dt);
	virtual bool Update(float dt);
	virtual bool PostUpdate(float dt);

	virtual void OnCollision(Collider* collider);
	virtual void CollisionPosUpdate();
	virtual Collider* GetCollider() const;

	SDL_Point GetPosition();
	ENTITY_TYPE GetType();

	void SetToDelete(bool toDelete);

protected:
	virtual void Draw(float dt);

public:
	bool started;
	bool toDelete;

	bool flip;

protected:
	SDL_Point position;

	ENTITY_TYPE type;

	SDL_Texture* texture;

	Collider* collider;

	Animation* current_animation;
};

#endif//__ENTITY_H__