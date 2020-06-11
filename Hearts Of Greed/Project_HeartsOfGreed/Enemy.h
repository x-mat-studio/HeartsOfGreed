#pragma once

#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "DynamicEntity.h"
#include "Animation.h"
#include "EntityEffects.h"

enum class ENEMY_STATES : int
{	
	UNKNOWN = -1,
	IDLE,

	MOVE,
	ATTACK,
	CHARGING_ATTACK,

	DEAD,

	MAX
};


enum class ENEMY_INPUTS : int
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


class Enemy : public DynamicEntity
{
public:

	Enemy(fMPoint position, ENTITY_TYPE type, Collider* collider, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
		Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightUp, Animation& idleRightDown, Animation& idleLeft, Animation& idleLeftUp, Animation& idleLeftDown,
		Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp, Animation& punchRightDown, Animation& punchRight,
		Animation& deathRight, Animation& deathRightUp, Animation& deathRightDown, Animation& deathLeft, Animation& deathLeftUp, Animation& deathLeftDown, int maxHitPoints, int currentHitPoints,
		int recoveryHitPointsRate, int vision, int attackDamage, float attackSpeed, int attackRange, int movementSpeed, int xpOnDeath, float scale = 1.0f);


	Enemy(fMPoint position, Enemy* copy, ENTITY_ALIGNEMENT align);

	~Enemy();


	bool Update(float dt);
	bool PostUpdate(float dt);


	void OnCollision(Collider* collider);

	void CheckObjective(Entity* entity);

	bool MoveTo(float x, float y);

	void Draw(float dt);


	void Die();

	int GetHP();
	int GetAD();
	float GetAS();
	int GetVision();
	int GetRecov();

	void DrawOnSelect();
	void DrawRangedVFX(float dt);

	int GetLongTermObjectiveX();
	int GetLongTermObjectiveY();

	void SetLongTermObjective(fMPoint point);

private:

	bool Attack();
	Frame GetAnimationCurrentFrame(float dt);


	void RecoverHealth();

	bool SearchObjective();
	bool CheckAttackRange();

	void InternalInput(std::vector<ENEMY_INPUTS>& inputs, float dt);
	bool ExternalInput(std::vector<ENEMY_INPUTS>& inputs, float dt);
	ENEMY_STATES ProcessFsm(std::vector<ENEMY_INPUTS>& inputs);

	int RecieveDamage(float damage, bool ignoreArmor = false);
	void SearchForNewObjective();

	void StateMachine(float dt);
	void SetAnimation(ENEMY_STATES state);

	void Roar();


public:

	EntityEffects debuffs;

private:

	int recoveryHitPointsRate;
	int vision;

	int attackDamage;
	float attackSpeed;
	int attackRange;

	int xpOnDeath;

	float attackCooldown;
	float attackRangeInPX;

	bool rangedWattack;

	bool haveOrders;
	Entity* shortTermObjective;
	fMPoint longTermObjective;

	Animation walkLeft;
	Animation walkLeftUp;
	Animation walkLeftDown;
	Animation walkRightUp;
	Animation walkRightDown;
	Animation walkRight;
	Animation idleRight;
	Animation idleRightUp;
	Animation idleRightDown;
	Animation idleLeft;
	Animation idleLeftUp;
	Animation idleLeftDown;
	Animation punchLeft;
	Animation punchLeftUp;
	Animation punchLeftDown;
	Animation punchRightUp;
	Animation punchRightDown;
	Animation punchRight;
	Animation deathRight;
	Animation deathRightUp;
	Animation deathRightDown;
	Animation deathLeft;
	Animation deathLeftUp;
	Animation deathLeftDown;

	Animation particleRanged;

	Animation* currentAnimation;

	ENEMY_STATES state;
	std::vector<ENEMY_INPUTS> inputs;

	float damageTakenTimer;
	float scale;

};


#endif //__ENEMY_H__
