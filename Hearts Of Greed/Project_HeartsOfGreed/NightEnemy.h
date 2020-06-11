#ifndef __NIGHT_ENEMY_H__
#define __NIGHT_ENEMY_H__

#include "Enemy.h"
#include "DynamicEntity.h"
#include "Animation.h"

class NightEnemy : public Enemy
{
public:
	NightEnemy(fMPoint position, ENTITY_TYPE type, Collider* collider, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
		Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightUp, Animation& idleRightDown, Animation& idleLeft, Animation& idleLeftUp, Animation& idleLeftDown,
		Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp, Animation& punchRightDown, Animation& punchRight,
		Animation& deathRight, Animation& deathRightUp, Animation& deathRightDown, Animation& deathLeft, Animation& deathLeftUp, Animation& deathLeftDown, int maxHitPoints, int currentHitPoints,
		int recoveryHitPointsRate, int vision, int attackDamage, int attackSpeed, int attackRange, int movementSpeed, int xpOnDeath, float scale);


	NightEnemy(fMPoint position, Enemy* copy, ENTITY_ALIGNEMENT align);

	~NightEnemy();

};


#endif //__NIGHT_ENEMY_H__
