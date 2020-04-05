#ifndef _TURRET_H_
#define _TURRET_H_

#include "Module.h"
#include "Building.h"
#include "Collision.h"
#include "Animation.h"

enum class TURRET_STATES
{
	UNKNOWN = -1,
	IDLE,

	ATTACK,
	IN_CHARGING_ATTACK,
	CHARGING_ATTACK,

	DEAD,

	MAX
};

enum class TURRET_INPUTS
{
	IN_IDLE,

	IN_ATTACK,
	IN_CHARGING_ATTACK,
	IN_ATTACK_CHARGED,

	IN_OUT_OF_RANGE,

	IN_OBJECTIVE_DONE,

	IN_DEAD
};

class Turret : public Building
{

public:

	Turret(int turretLvl, int attackDmg, int attackSpeed, int range,fMPoint position, Collider* collider, Animation& animation, int hitPoints=100,int recoveryHitPointsRate=5, int xpOnDeath=100, int buildingCost=50, int transparency = 0);
	Turret(Turret* copy);
	~Turret();


	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	void CheckObjecive(Entity* entity);
	bool SearchObjective();
	bool CheckAttackRange();
	void Attack();
	void Die();

	Entity* EnemyInRange();

	void InternalInput(std::vector<TURRET_INPUTS>& inputs, float dt);
	bool ExternalInput(std::vector<TURRET_INPUTS>& inputs, float dt);
	TURRET_STATES ProcessFsm(std::vector<TURRET_INPUTS>& inputs);

	void StateMachine();

private:
	int turretLvl;
	int attackDmg;
	int attackSpeed;
	float attackCD;
	int range;

private:
	bool haveOrders;
	Entity* shortTermObjective;
	iMPoint longTermObjective;
	Animation animation;
	TURRET_STATES state;
	Animation* currentAnimation;
	std::vector<TURRET_INPUTS> inputs;
};



#endif // _TURRET_H_
