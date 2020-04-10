#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "SDL/include/SDL_rect.h"
#include "Collision.h"
#include "Audio.h"
#include "p2Point.h"
#include "Animation.h"
#include "FoWEntity.h"

#define MAX_ENTITY_TYPES 13

struct Collider;
struct SDL_Texture;


enum class ENTITY_TYPE : int

{
	UNKNOWN = -1,

	PARTICLE,
	EMITER,
	PARTICLE_SYSTEM,

	SPAWNER,

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

enum class ENTITY_ALIGNEMENT : int
{
	UNKNOWN = -1,
	PLAYER,
	ENEMY,
	NEUTRAL,


	MAX_ALIGN
};

class Entity
{
public:

	Entity();
	Entity(fMPoint position, ENTITY_TYPE type, ENTITY_ALIGNEMENT alignement, Collider* collider, bool dynamic = false);
	virtual ~Entity();

	virtual bool Start(SDL_Texture* texture);
	virtual bool PreUpdate(float dt);
	virtual bool Update(float dt);
	virtual bool PostUpdate(float dt);

	virtual void OnCollision(Collider* collider);
	virtual void CollisionPosUpdate();
	virtual Collider* GetCollider() const;

	virtual int RecieveDamage(int damage);

	fMPoint GetPosition();
	fMPoint GetCenter();
	void SetPosition(int x, int y);

	void SetTexture(SDL_Texture* texture);
	SDL_Texture* GetTexture();

	SDL_Rect GetAnimationRect(float dt);
	ENTITY_TYPE GetType();
	ENTITY_ALIGNEMENT GetAlignment();
	bool IsOpositeAlignement(ENTITY_ALIGNEMENT check);
	void SetAlignment(ENTITY_ALIGNEMENT newAlign = ENTITY_ALIGNEMENT::UNKNOWN);

	void SetToDelete(bool toDelete);
	virtual void Draw(float dt);	

	//Sound related
	DIRECTION GetMyDirection();
	LOUDNESS GetMyLoudness();

public:
	bool started;
	bool toDelete;

	bool flip;
	bool dynamic;
	bool UIAssigned;
	FoWEntity* visionEntity;

protected:
	fMPoint position;
	fMPoint offset;
	fMPoint center;

	ENTITY_TYPE type;
	ENTITY_ALIGNEMENT align;

	SDL_Texture* texture;

	Collider* collider;

};

#endif//__ENTITY_H__