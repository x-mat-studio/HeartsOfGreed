#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "SDL/include/SDL_rect.h"
#include "Collision.h"
#include "Audio.h"
#include "p2Point.h"
#include "Animation.h"
#include "FoWEntity.h"
#include "Minimap.h"

#define MAX_ENTITY_TYPES 17

struct Collider;
struct SDL_Texture;

//The elements of this enum are ordered by selection priority being the first one more important than the last one
enum class ENTITY_TYPE : int

{
	UNKNOWN = -1,

	
	HERO_MELEE,
	HERO_RANGED,
	HERO_GATHERER,
	HERO_ROBO,
	
	ENEMY,
	ENEMY_RANGED,
	ENEMY_NIGHT,
	ENEMY_GIGA,

	BLDG_TURRET,
	BLDG_UPGRADE_CENTER,
	BLDG_BASE,
	BLDG_BARRICADE,

	

	PARTICLE_SYSTEM,

	SPAWNER,


	QUEST,
	HQ_COMANDER,

	BUILDING,

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

enum class SKILL_ID : int
{
	NO_TYPE = -1,

	GATHERER_GRENADE,
	MELEE_HAMMER,
	RANGED_SLOW_GRENADE,
	ROBOTTO_KABOOM
};

enum class SKILL_TYPE
{
	NO_TYPE = -1,

	SINGLE_TARGET,
	AREA_OF_EFFECT
};

enum SKILL_EFFECT : int
{
	NO_EFFECT = -1,

	SLOWDOWN,
	BLOOD_LOSS,
	KNOCKDOWN,

	EFFECT_ALL
};


class Entity
{
public:

	Entity();
	Entity(fMPoint position, ENTITY_TYPE type, ENTITY_ALIGNEMENT alignement, Collider* collider, int maxHealth, int currentHealth, bool dynamic = false);
	virtual ~Entity();

	virtual bool Start(SDL_Texture* texture);
	virtual bool PreUpdate(float dt);
	virtual bool Update(float dt);
	virtual bool PostUpdate(float dt);

	virtual void OnCollision(Collider* collider);
	virtual void CollisionPosUpdate();
	virtual Collider* GetCollider() const;
	void SetColliderRect(SDL_Rect newRect);
	void ActiveCollider();

	virtual int RecieveDamage(float damage, bool ignoreArmor = false);
	virtual void CheckObjective(Entity* deleted);

	fMPoint GetPosition();
	int GetRadiusSize();
	fMPoint GetCenter();
	fMPoint GetOffset();
	void SetPosition(int x, int y);

	virtual int GetMaxHP() const;
	virtual void SetMaxHP(int newMaxHp) ;

	virtual int GetCurrentHP() const;
	virtual void SetCurrentHP(int newcurrHp);

	void SetTexture(SDL_Texture* texture);
	SDL_Texture* GetTexture();

	SDL_Rect GetAnimationRect(float dt);
	ENTITY_TYPE GetType();
	ENTITY_ALIGNEMENT GetAlignment();
	bool IsOpositeAlignement(ENTITY_ALIGNEMENT check);
	virtual void SetAlignment(ENTITY_ALIGNEMENT newAlign = ENTITY_ALIGNEMENT::UNKNOWN);

	void SetToDelete(bool toDelete);
	virtual void Draw(float dt);	
	virtual void MinimapDraw(float scale, float halfWidth);
	virtual void DebugDraw();
	virtual void ApplyEffect();

	//Sound related
	DIRECTION GetMyDirection();
	LOUDNESS GetMyLoudness();

public:
	bool started;
	bool toDelete;

	bool flip;
	bool dynamic;
	bool UIAssigned;

	bool selectedByPlayer;
	bool missionEntity;

	FoWEntity* visionEntity;
	MinimapIcon* minimapIcon;
	fMPoint position;

	float hitPointsMax;
	float hitPointsCurrent;

protected:
	
	fMPoint offset;
	fMPoint center;

	int radiusSize;

	ENTITY_TYPE type;
	ENTITY_ALIGNEMENT align;

	SDL_Texture* texture;

	Collider* collider;

};

#endif//__ENTITY_H__