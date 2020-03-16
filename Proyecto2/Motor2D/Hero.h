#pragma once

#ifndef __HERO_H__
#define __HERO_H__

#include "Entity.h"
#include "Animation.h"

enum class HERO_STATES
{
	UNKNOWN,
	IDLE,

	MOVE,
	ATTACK,
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
	IN_SKILL1,
	IN_SKILL2,
	IN_SKILL3,
	
	IN_SKILL_FINISHED,

	IN_OUT_OF_RANGE,
	
	IN_OBJECTIVE_DONE,

	IN_ATTACKED,
	IN_DEAD
};

class Hero : public Entity
{
public:
	Hero(SDL_Point position, ENTITY_TYPE type, SDL_Rect collRect, COLLIDER_TYPE collType, Module* callback,
		Animation& animation1, Animation& animation2, Animation& animation3, Animation& animation4, 
		Animation& animation5, Animation& animation6, Animation& animation7, Animation& animation8, 
		Animation& animation9, Animation& animation10, Animation& animation11, Animation& animation12,
		int level, int hitPoints,int recoveryHitPointsRate,int energyPoints, int recoveryEnergyRate,
		int attackDamage, int attackSpeed, int attackRange, int movementSpeed, int vision,float attackCooldown, 
		float coolDownHability1, float coolDownHability2, float coolDownHability3);

	Hero(SDL_Point position, Hero* copy);
	~Hero();

	bool MoveTo(int x, int y);
	bool LockOn(Entity*);

	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	void OnCollision(Collider* collider);

	virtual bool UseHability1();
	virtual bool UseHability2();
	virtual bool UseHability3();


private:
	void LevelUp();

	void Draw(float dt);

	void Move();
	bool CheckRange(int maxDistance);
	void Attack();
	void Die();

	void RecoverHealth();
	void RecoverEnergy();

	void internal_input(std::vector<HERO_INPUTS>& inputs, float dt);
	bool external_input(std::vector<HERO_INPUTS>& inputs, float dt);
	HERO_STATES process_fsm(std::vector<HERO_INPUTS>& inputs);

private:
	int level;

	int hitPoints;
	int recoveryHitPointsRate;
	int energyPoints;
	int recoveryEnergyRate;

	int attackDamage;
	int attackSpeed;
	int attackRange;

	int movementSpeed;
	int vision;

	float attackCooldown;
	float coolDownHability1;
	float coolDownHability2;
	float coolDownHability3;

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
};


#endif//__HERO_H__