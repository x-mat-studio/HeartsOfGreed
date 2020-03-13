#pragma once

#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "vector"

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
	BLDG_TURRET,
	BLDG_UPGRADE,
	BLDG_BASE,
	BLDG_BARRICADE,

	MAX_TYPE
};

class Entity
{
public:

	Entity();
	Entity(std::vector<int>& position, ENTITY_TYPES type, SDL_Texture* texture);
	virtual ~Entity();

	virtual bool Start();
	virtual bool PreUpdate(float dt);
	virtual bool Update(float dt);
	virtual bool PostUpdate(float dt);
	
	virtual void OnCollision(Collider* collider);
	virtual Collider* GetCollider() const;

	bool started;
	bool toDelete;

protected:
	virtual void Draw(float dt);

public:
	bool started;
	bool toDelete;

protected:
	std::vector<int> position;
	
	ENTITY_TYPE type;

	SDL_Texture* texture;


};



#endif//__ENTITY_H__