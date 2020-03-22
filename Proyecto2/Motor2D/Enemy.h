#pragma once

#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Animation.h"

enum class ENEMY_STATES
{
	UNKNOWN = -1,
	IDLE,

	MOVE,
	ATTACK,
	CHARGING_ATTACK,

	DEAD,

	MAX
};


enum class ENEMY_INPUTS
{
	IN_IDLE,
	IN_MOVE,
	
	IN_ATTACK,
	IN_CHARGING_ATTACK,
	IN_ATTACK_CHARGED,

	IN_OUT_OF_RANGE,

	IN_OBJECTIVE_DONE,

	IN_DEAD
};


class Enemy : public Entity
{
public:

	Enemy(iMPoint position, ENTITY_TYPE type, SDL_Texture* texture, Collider* col,
		Animation& animation, int hitPoints, int recoveryHitPointsRate, int attackDamage, int attackSpeed, int attackRange,
		int movementSpeed, int xpOnDeath);


	Enemy(iMPoint position, Enemy* copy);


	~Enemy();


	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);


	void OnCollision(Collider* collider);

	bool MoveTo(int x, int y);
	bool Enemy::LockOn(Entity* entity);

private:

	void Draw(float dt);

	void Move();
	void Attack();
	void Die();

	void RecoverHealth();

	void internalInput(std::vector<ENEMY_INPUTS>& inputs, float dt);
	bool externalInput(std::vector<ENEMY_INPUTS>& inputs, float dt);
	ENEMY_STATES processFsm(std::vector<ENEMY_INPUTS>& inputs);

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

	Animation animation;

	ENEMY_STATES state;
	std::vector<ENEMY_INPUTS> inputs;

};


#endif //__ENEMY_H__
