#pragma once

#ifndef __HERO_H__
#define __HERO_H__

#define TIME_TRIGGER 0.001f

#include "Animation.h"
#include "DynamicEntity.h"


enum class HERO_STATES
{
	UNKNOWN = -1,
	IDLE,

	MOVE,
	ATTACK,
	CHARGING_ATTACK,

	PREPARE_SKILL1,
	SKILL1,
	PREPARE_SKILL2,
	SKILL2,
	PREPARE_SKILL3,
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

	IN_PREPARE_SKILL1,
	IN_SKILL1,
	IN_PREPARE_SKILL2,
	IN_SKILL2, 
	IN_PREPARE_SKILL3,
	IN_SKILL3,
	
	IN_SKILL_CANCEL,
	IN_SKILL_FINISHED,

	IN_OUT_OF_RANGE,
	
	IN_OBJECTIVE_DONE,

	IN_ATTACKED,
	IN_DEAD
};



struct Skill
{
	Skill(SKILL_ID id, int dmg, SKILL_TYPE type, ENTITY_ALIGNEMENT target);
	Skill(const Skill& skill1);

	int dmg;

	ENTITY_ALIGNEMENT target;
	SKILL_TYPE type;
	SKILL_ID id;

	// For the Future (?)
	//EFFECT effect = EFFECT::KNOCKDOWN
};

struct skillArea;

class Hero : public DynamicEntity
{
public:
	Hero(fMPoint position, ENTITY_TYPE type, Collider* col,
		Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
		Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightUp,
		Animation& idleRightDown, Animation& idleLeft, Animation& idleLeftUp, Animation& idleLeftDown,
		Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp,
		Animation& punchRightDown, Animation& punchRight, Animation& skill1Right, Animation& skill1RightUp,
		Animation& skill1RightDown, Animation& skill1Left, Animation& skill1LeftUp, Animation& skill1LeftDown, 
		Animation& deathRight, Animation& deathRightUp, Animation& deathRightDown, Animation& deathLeft, Animation& deathLeftUp, Animation& deathLeftDown, Animation& tileOnWalk,
		int level, int maxHitPoints, int currentHitPoints, int recoveryHitPointsRate, int maxEnergyPoints, int energyPoints, int recoveryEnergyRate,
		int attackDamage, float attackSpeed, int attackRange, int movementSpeed, int vision, float skill1ExecutionTime,
		float skill2ExecutionTime, float skill3ExecutionTime, float skill1RecoverTime, float skill2RecoverTime, float skill3RecoverTime,
		int skill1Dmg, SKILL_ID skill1Id, SKILL_TYPE skill1Type, ENTITY_ALIGNEMENT skill1Target);

	Hero(fMPoint position, Hero* copy, ENTITY_ALIGNEMENT alignement);

	virtual ~Hero();

	bool MoveTo(int x, int y, bool haveObjective = true);
	bool LockOn(Entity*);

	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	virtual void OnCollision(Collider* collider);

	virtual void LevelUp();

	int RecieveDamage(int damage);

	bool GetExperience(int xp);
	bool GetLevel();

	void CheckObjective(Entity* entity);
	void Draw(float dt);
	void DrawArea();

	//Skill Related-----------------
	// Tells the Hero to get in the launch skill state
	bool PrepareSkill1();
	bool PrepareSkill2();
	bool PrepareSkill3();

	//Generates Area for the skill
	virtual bool PreProcessSkill1();
	virtual bool PreProcessSkill2();
	virtual bool PreProcessSkill3();

	//Checks if the skill can be launch, if so, launches it
	virtual bool ActivateSkill1(fMPoint mouseClick);
	virtual bool ActivateSkill2();
	virtual bool ActivateSkill3();

	// Tells the manager that a skill has been launched
	virtual bool ExecuteSkill1();
	virtual bool ExecuteSkill2();
	virtual bool ExecuteSkill3();

	void DrawSelected();

	//This is a placeholder for particles
	virtual bool DrawVfx(float dt);
	bool drawingVfx;

	void SkillCanceled();

	virtual void PlayGenericNoise(int random);

	//Getters and setters

	int GetHeroLevel() const;
	void SetHeroLevel(int level);

	int GetExpToLevelUp() const;
	void SetExpToLevelUp(int exp);

	int GetHeroXP() const;
	void SetHeroXP(int xp);

	int GetRecoveryHitPointsRate() const;
	void SetRecoveryHitPointsRate(int hpRate);

	int GetEnergyPoints() const;
	void SetEnergyPoints(int energyPoints);

	int GetMaxEnergyPoints() const;
	void SetMaxEnergyPoints(int maxEnergyPoints);

	int GetRecoveryEnergyRate() const;
	void SetRecoveryEnergyRate(int recoveryEnergyRate);

	float GetRecoveringHealth() const;
	void SetRecoverHealth(float recoverHealth);

	float GetRecoveringEnergy() const;
	void SetRecoveringEnergy(float recoverEnergy);

	float GetFeelingSecure() const;
	void SetFeelingSecure(float feelingSecure);

	int GetAttackDamage() const;
	void SetAttackDamage(int atkDamage);

	int GetAttackRange() const;
	void SetAttackRange(int atkRange);

	int GetSkill1Cost() const;
	void SetSkill1Cost(int skillCost);

	float GetAttackSpeed() const;
	void SetAttackSpeed(float atkSpeed);

	float GetSkill1RecoverTime() const;
	void SetSkill1RecoverTime(float skillRecoverTime);

	float GetSkill2RecoverTime() const;
	void SetSkill2RecoverTime(float skillRecoverTime);


	float GetSkill3RecoverTime() const;
	void SetSkill3RecoverTime(float skillRecoverTime);


	float GetSkill1TimePassed() const;
	float GetSkill2TimePassed() const;
	float GetSkill3TimePassed() const;


	int GetVisionDistance() const;
	void SetVisionDistance(int visDistance);

	float GetVisionInPx() const;
	void SetVisionInPx(float visPx);

private:
	
	bool CheckAttackRange();
	void Attack();
	void Die();

	void RecoverHealth(float dt);
	void RecoverEnergy(float dt);

	void InternalInput(std::vector<HERO_INPUTS>& inputs, float dt);
	HERO_STATES ProcessFsm(std::vector<HERO_INPUTS>& inputs);

	void StateMachine(float dt);

	void SearchForNewObjective();

	void FeelingSecure(float dt);

	void CommandVfx(float dt);
	virtual void BlitCommandVfx(Frame& currframe, int alphaValue);

	virtual void PlayOnHitSound();

protected:
	void SetAnimation(HERO_STATES currState);

public:
	int level;
	int expToLevelUp;
	int heroXP;

	int recoveryHitPointsRate;
	int energyPoints;
	int maxEnergyPoints;
	int recoveryEnergyRate;

	float recoveringHealth;
	float recoveringEnergy;
	float feelingSecure;

	int attackDamage;
	int attackRange;

	bool gettingAttacked;

	int skill1Cost;

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

	bool skillExecutionDelay;

	int visionDistance;

	float attackCooldown;
	float cooldownHability1;
	float cooldownHability2;
	float cooldownHability3;

	float visionInPx;

	Entity* objective;

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
	Animation skill1Right;
	Animation skill1RightUp;
	Animation skill1RightDown;
	Animation skill1Left;
	Animation skill1LeftUp;
	Animation skill1LeftDown;
	Animation deathRight;
	Animation deathRightUp;
	Animation deathRightDown;
	Animation deathLeft;
	Animation deathLeftUp;
	Animation deathLeftDown;
	Animation tileOnWalk;


	bool skillFromAttacking;
	HERO_STATES state;

	std::vector<HERO_INPUTS> inputs;
	Animation *currentAnimation;

	int framesPerPathfinding;
	int framePathfindingCount;

	bool godMode;

	std::vector <iMPoint> currAoE;
	std::vector <iMPoint> suplAoE;

	skillArea* currAreaInfo;
	Skill skill1;

	float damageTakenTimer;

	iMPoint movingTo;

};


#endif//__HERO_H__