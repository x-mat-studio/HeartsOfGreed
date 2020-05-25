#ifndef _TURRET_H_
#define _TURRET_H_

#include "Module.h"
#include "Building.h"
#include "Collision.h"
#include "Animation.h"
#include "DynamicEntity.h"


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

	Turret(int turretLvl, int attackDmg, int attackSpeed, int range, int vision, fMPoint position, Collider* collider, Animation& idleRight, Animation& idleRightUp, Animation& idleRightDown, Animation& idleLeft,
		Animation& idleLeftUp, Animation& idleLeftDown, Animation& shootingRight, Animation& shootingRightUp, Animation& shootingRightDown, Animation& shootingLeft, Animation& shootingLeftUp,
		Animation& shootingLeftDown, int maxHitPoints = 100, int currentHitPoints = 100, int recoveryHitPointsRate=5, int xpOnDeath=100, int buildingCost=50, int transparency = 0);

	Turret(fMPoint position, Turret* copy, ENTITY_ALIGNEMENT alignement);

	~Turret();


	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	void Draw(float dt);

	int GetLvl();
	int GetAD();
	int GetAS();
	int GetRng();

	void SetLevel(int level);

	void DrawSelected();

	int RecieveDamage(float damage);


private:

	void CheckObjective(Entity* entity);
	bool SearchObjective();
	bool CheckAttackRange();
	void Attack();
	void Die();

	Entity* EnemyInRange();

	void InternalInput(std::vector<TURRET_INPUTS>& inputs, float dt);
	bool ExternalInput(std::vector<TURRET_INPUTS>& inputs, float dt);
	TURRET_STATES ProcessFsm(std::vector<TURRET_INPUTS>& inputs);

	void Turret::SetAnimation(TURRET_STATES state);
	FACE_DIR DetermineDirection(fMPoint dir);

	void StateMachine();

private:
	int turretLvl;
	int attackDmg;
	int attackSpeed;
	float attackCD;
	int range;
	int vision;

	Animation idleRight;
	Animation idleRightUp;
	Animation idleRightDown;
	Animation idleLeft;
	Animation idleLeftUp;
	Animation idleLeftDown;
	Animation shootingRight;
	Animation shootingRightUp;
	Animation shootingRightDown;
	Animation shootingLeft;
	Animation shootingLeftUp;
	Animation shootingLeftDown;

	Entity* shortTermObjective;
	TURRET_STATES state;
	Animation* currentAnimation;
	std::vector<TURRET_INPUTS> inputs;

	FACE_DIR dir;
};



#endif // _TURRET_H_
