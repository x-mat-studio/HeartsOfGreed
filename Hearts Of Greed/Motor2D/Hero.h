#pragma once

#ifndef __HERO_H__
#define __HERO_H__

#define TIME_TRIGGER 0.001

#include "Animation.h"
#include "DynamicEntity.h"

enum class HERO_STATES
{
	UNKNOWN = -1,
	IDLE,

	MOVE,
	ATTACK,
	CHARGING_ATTACK,
	SKILL1,
	SKILL2,
	SKILL3,

	REPAIR,

	DEAD,

	MAX
};


enum HERO_INPUTS
{
	IN_IDLE,
	IN_MOVE,
	IN_REPAIR,
	IN_ATTACK,
	IN_CHARGING_ATTACK,
	IN_ATTACK_CHARGED,

	IN_SKILL1,
	IN_SKILL2, 
	IN_SKILL3,
	
	IN_SKILL_FINISHED,

	IN_OUT_OF_RANGE,
	
	IN_OBJECTIVE_DONE,

	IN_ATTACKED,
	IN_DEAD
};

class Hero : public DynamicEntity
{
public:
	Hero(fMPoint position, ENTITY_TYPE type, Collider* col,
		Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
		Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightDown,
		Animation& idleRightUp, Animation& idleLeft, Animation& idleLeftUp, Animation& idleLeftDown,
		int level, int hitPoints, int recoveryHitPointsRate, int energyPoints, int recoveryEnergyRate,
		int attackDamage, int attackSpeed, int attackRange, int movementSpeed, int vision, float skill1ExecutionTime,
		float skill2ExecutionTime, float skill3ExecutionTime, float skill1RecoverTime, float skill2RecoverTime, float skill3RecoverTime);

	Hero(fMPoint position, Hero* copy, ENTITY_ALIGNEMENT alignement);
	~Hero();

	bool MoveTo(int x, int y, bool haveObjective = true);
	bool LockOn(Entity*);

	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	virtual void OnCollision(Collider* collider);

	virtual bool UseHability1();
	virtual bool UseHability2();
	virtual bool UseHability3();

	int RecieveDamage(int damage);

	void CheckObjecive(Entity* entity);
	void Draw(float dt);

private:
	void LevelUp();
	
	bool CheckAttackRange();
	void Attack();
	void Die();

	void RecoverHealth();
	void RecoverEnergy();

	void InternalInput(std::vector<HERO_INPUTS>& inputs, float dt);
	HERO_STATES ProcessFsm(std::vector<HERO_INPUTS>& inputs);

	void StateMachine(float dt);

	void SearchForNewObjective();

protected:
	void SetAnimation(HERO_STATES currState);

public:
	int level;

	int hitPoints;
	int recoveryHitPointsRate;
	int energyPoints;
	int recoveryEnergyRate;

	int attackDamage;
	int attackRange;

	float attackSpeed;
	float skill1RecoverTime;
	float skill2RecoverTime;
	float skill3RecoverTime;


	float skill1ExecutionTime;
	float skill2ExecutionTime;
	float skill3ExecutionTime;

	float skill1TimePassed;
	float skill2TimePassed;
	float skill3TimePassed;


	bool skill1Charged;
	bool skill2Charged;
	bool skill3Charged;

	int movementSpeed;
	int visionDistance;

	float attackCooldown;
	float cooldownHability1;
	float cooldownHability2;
	float cooldownHability3;

	Entity* objective;
	bool selected;

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
	

	bool skillFromAttacking;
	HERO_STATES state;

	std::vector<HERO_INPUTS> inputs;
	Animation *currentAnimation;

	int framesPerPathfinding;
	int framePathfindingCount;
};


#endif//__HERO_H__