#ifndef __GIGA_ENEMY_H__
#define __GIGA_ENEMY_H__

#include "Enemy.h"
#include "DynamicEntity.h"
#include "Animation.h"

class GigaEnemy : public Enemy
{
public:
	GigaEnemy(fMPoint position, ENTITY_TYPE type, Collider* collider, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
		Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightUp, Animation& idleRightDown, Animation& idleLeft, Animation& idleLeftUp, Animation& idleLeftDown,
		Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp, Animation& punchRightDown, Animation& punchRight,
		Animation& deathRight, Animation& deathRightUp, Animation& deathRightDown, Animation& deathLeft, Animation& deathLeftUp, Animation& deathLeftDown, int maxHitPoints, int currentHitPoints,
		int recoveryHitPointsRate, int vision, int attackDamage, float attackSpeed, int attackRange, int movementSpeed, int xpOnDeath, float scale);


	GigaEnemy(fMPoint position, Enemy* copy, ENTITY_ALIGNEMENT align);

	~GigaEnemy();

};


#endif //__RANGED_ENEMY_H__
