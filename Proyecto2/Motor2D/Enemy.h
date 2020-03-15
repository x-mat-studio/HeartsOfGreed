#pragma once

#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Animation.h"

enum class ENEMY_STATE : int
{
	ST_UNKNOWN = -1,

	ST_IDLE,
	ST_CHASING,

	ST_ALL,
};

class Enemy : public Entity
{
public:

	Enemy(SDL_Point position, ENTITY_TYPE type, SDL_Texture* texture, SDL_Rect collRect, COLLIDER_TYPE collType, Module* callback,
		Animation& animation, int hitPoints, int recoveryHitPointsRate, int attackDamage, int attackSpeed, int attackRange,
		int movementSpeed, int xpOnDeath, float attackCooldown);

	Enemy(SDL_Point position, Enemy* copy);

	//Destructor
	~Enemy();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);


	void OnCollision(Collider* collider);

	bool MoveTo(int x, int y);
	bool LockOn(Entity*);

private:

	void Draw(float dt);

	void Move();
	void Attack();
	void Die();

	void RecoverHealth();

private:

	int hitPoints;
	int recoveryHitPointsRate;

	int attackDamage;
	int attackSpeed;
	int attackRange;

	int movementSpeed;
	int xpOnDeath;

	float attackCooldown;

	Entity* objective;
	bool selected;

	Animation animation;

};


#endif // !
