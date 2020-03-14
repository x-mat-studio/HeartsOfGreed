#pragma once

#ifndef __HERO_H__
#define __HERO_H__

#include "Entity.h"
#include "Animation.h"


class Hero : public Entity
{
public:
	Hero(SDL_Point position, ENTITY_TYPE type, SDL_Texture* texture, Animation& animation, int level, int hitPoints, int recoveryHitPointsRate,
		int energyPoints, int recoveryEnergyRate, int attackDamage, int attackSpeed, int attackRange, int movementSpeed, int vision,
		float attackCooldown, float coolDownHability1, float coolDownHability2, float coolDownHability3);

	Hero(SDL_Point position, Hero* copy);
	~Hero();

	bool MoveTo(int x, int y);
	bool LockOn(Entity*);

	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	void OnCollision(Collider* collider);
	Collider* GetCollider() const;

	//virtual bool UseHability1();
	//virtual bool UseHability2();
	//virtual bool UseHability3();

private:
	void LevelUp();
	
	void Move();
	bool CheckRange(int maxDistance);
	void Attack();
	void Die();

	void RecoverHealth();
	void RecoverEnergy();

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

	Animation animation;

	Collider* collider;
};


#endif//__HERO_H__