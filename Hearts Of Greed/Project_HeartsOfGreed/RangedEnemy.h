#ifndef __RANGED_ENEMY_H__
#define __RANGED_ENEMY_H__

#include "Enemy.h"
#include "DynamicEntity.h"
#include "Animation.h"
#include "EasingFunctions.h"

class RangedEnemy : public Enemy
{
public:
	RangedEnemy(fMPoint position, ENTITY_TYPE type, Collider* collider, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
		Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightUp, Animation& idleRightDown, Animation& idleLeft, Animation& idleLeftUp, Animation& idleLeftDown,
		Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp, Animation& punchRightDown, Animation& punchRight,
		Animation& deathRight, Animation& deathRightUp, Animation& deathRightDown, Animation& deathLeft, Animation& deathLeftUp, Animation& deathLeftDown, Animation& particleRanged,
		int maxHitPoints, int currentHitPoints,int recoveryHitPointsRate, int vision, int attackDamage, int attackSpeed, int attackRange, int movementSpeed, int xpOnDeath, float scale);


	RangedEnemy(fMPoint position, Enemy* copy, ENTITY_ALIGNEMENT align);

	~RangedEnemy();

	void LaunchProjectile();
	void UpdateProjectile(float dt);
	void DestroyProjectile();

private:

	void DrawVFX(float dt);

private:
	Animation rangedAttack;
	Frame* projectile;
	fMPoint projectilePos;
	fMPoint projectileDestination;
	Easing projectileEasingX;
	Easing projectileEasingY;

	double projectileStartedAt;

};


#endif //__RANGED_ENEMY_H__
