#pragma once

#ifndef __HERO_H__
#define __HERO_H__

#define TIME_TRIGGER 0.001f

#include "Animation.h"
#include "DynamicEntity.h"
#include "ParticleSystem.h"


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
	Skill();
	Skill(SKILL_ID id, int dmg, int cooldown, int rangeRadius, int attackRadius, bool hurtYourself, float executionTime, SKILL_TYPE type, ENTITY_ALIGNEMENT target, int lvl, int energyCost, SKILL_EFFECT effect = SKILL_EFFECT::NO_EFFECT);
	Skill(const Skill& skill1);

	Skill operator= (Skill& newSkill);


	int lvl;
	int dmg;
	int coolDown;
	float executionTime;
	int energyCost;

	int rangeRadius;
	int attackRadius;

	bool hurtYourself;

	ENTITY_ALIGNEMENT target;
	SKILL_TYPE type;
	SKILL_ID id;

	SKILL_EFFECT effect;
};

struct skillArea;

struct HeroStats
{
	HeroStats();
	HeroStats(HeroStats& newStats);
	HeroStats operator=(HeroStats& newStats);


	int maxHP;
	float damage;
	int maxEnergy;
	float atkSpeed;
	float recoveryHPRate;
	float recoveryEnergyRate;
	int heroLevel;
	int movSpeed;
	int visionDistance;
	float attackRange;
	int xpToLvlUp;

	int currHP;
	int currEnergy;
};

class DeadHero
{
public:
	DeadHero(int level, ENTITY_TYPE type, Skill& skill, Skill& passiveSkill);
	~DeadHero();

	ENTITY_TYPE GetType()const;
	int GetLevel()const;
	
	void GetSkillInfo(SKILL_ID& id, int& skillLevel) const;
	void GetPassiveSkillInfo(SKILL_ID& id, int& skillLevel) const;

private:
	ENTITY_TYPE heroType;
	int level;
	SKILL_ID skillId;
	int skillLevel;

	SKILL_ID passiveSkillId;
	int passiveSkillLevel;


};

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
		HeroStats& stats, Skill& skill1);

	Hero(fMPoint position, Hero* copy, ENTITY_ALIGNEMENT alignement);

	virtual ~Hero();

	bool MoveTo(int x, int y, bool haveObjective = true);
	bool LockOn(Entity*);

	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	virtual void OnCollision(Collider* collider);

	virtual void LevelUp();

	int RecieveDamage(float damage);

	bool GetExperience(int xp);
	bool GetLevel();

	void CheckObjective(Entity* entity);
	void Draw(float dt);
	void DrawArea();


	virtual void UpdatePasiveSkill(float dt);

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
	void AddEnergyPoints(int engPoints);

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

	float GetAttackDamage() const;
	void SetAttackDamage(float atkDamage);

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

	int GetMaxHP() const;
	void SetMaxHP(int newMaxHp);

	int GetCurrentHP() const;
	void SetCurrentHP(int newcurrHp);

	float GetSkill1TimePassed() const;
	float GetSkill2TimePassed() const;
	float GetSkill3TimePassed() const;


	int GetVisionDistance() const;
	void SetVisionDistance(int visDistance);

	float GetVisionInPx() const;
	void SetVisionInPx(float visPx);

	Skill GetSkill1() const;
	void SetSkill(Skill skill);
	virtual Skill GetPassiveSkill() const;

	void ReplaceSkill1(Skill& newSkill);
	virtual void ReplacePassiveSkill(Skill& skill);

	void ReplaceHeroStats(HeroStats newStats);

	int GetHeroSkillPoints();
	void SetHeroSkillPoints(int n);
	void AddHeroSkillPoints(int n);

	bool IsDying();

protected:
	void SetAnimation(HERO_STATES currState);
	void HandleMyParticleSystem(float dt);
	void TimeMyParticleSystem(float dt);
	void ResetAttackAnimation();

	virtual void Die();
	void ExecuteSFX(int sfx);

	void RecoverHealth(float dt);

private:
	void ResetBonusStats();

	bool CheckAttackRange();
	Frame GetAnimationCurrentFrame(float dt);
	virtual void Attack();

	void RecoverEnergy(float dt);

	void InternalInput(std::vector<HERO_INPUTS>& inputs, float dt);
	HERO_STATES ProcessFsm(std::vector<HERO_INPUTS>& inputs);

	void StateMachine(float dt);

	void SearchForNewObjective();

	void FeelingSecure(float dt);

	void CommandVfx(float dt);
	virtual void BlitCommandVfx(Frame& currframe, int alphaValue);

	virtual void PlayOnHitSound();



public:
	bool skill1Charged;
	bool skill2Charged;
	bool skill3Charged;

	float bonusArmor;
	float bonusAttack;

	bool godMode;

protected:

	HeroStats stats;

	float recoveringHealth;
	float recoveringEnergy;
	float feelingSecure;

	int heroXP;

	bool gettingAttacked;


	float skill1RecoverTime;
	float skill2RecoverTime;
	float skill3RecoverTime;


	float skill1ExecutionTime;
	float skill2ExecutionTime;
	float skill3ExecutionTime;

	float skill1TimePassed;
	float skill2TimePassed;
	float skill3TimePassed;

	bool skillExecutionDelay;


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
	Animation* currentAnimation;

	int framesPerPathfinding;
	int framePathfindingCount;

	std::vector <iMPoint> currAoE;
	std::vector <iMPoint> suplAoE;

	skillArea* currAreaInfo;
	Skill skill1;

	float damageTakenTimer;

	iMPoint movingTo;

	int heroSkillPoints;

	ParticleSystem* myParticleSystem;
	float lvlUpSfxTimer;

	bool comeFromAttack;
};


#endif//__HERO_H__