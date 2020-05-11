#include "RangedEnemy.h"

RangedEnemy::RangedEnemy(fMPoint position, ENTITY_TYPE type, Collider* collider, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
	Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightUp, Animation& idleRightDown, Animation& idleLeft, Animation& idleLeftUp, Animation& idleLeftDown,
	Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp, Animation& punchRightDown, Animation& punchRight,
	Animation& deathRight, Animation& deathRightUp, Animation& deathRightDown, Animation& deathLeft, Animation& deathLeftUp, Animation& deathLeftDown, int maxHitPoints, int currentHitPoints,
	int recoveryHitPointsRate, int vision, int attackDamage, int attackSpeed, int attackRange, int movementSpeed, int xpOnDeath, float scale) :

	Enemy(position, type,  collider, walkLeft,  walkLeftUp,  walkLeftDown,  walkRightUp,
		 walkRightDown,  walkRight,  idleRight,  idleRightUp,  idleRightDown,  idleLeft,  idleLeftUp,  idleLeftDown,
		 punchLeft,  punchLeftUp,  punchLeftDown,  punchRightUp,  punchRightDown,  punchRight,
		 deathRight,  deathRightUp,  deathRightDown,  deathLeft,  deathLeftUp,  deathLeftDown, maxHitPoints, currentHitPoints,
		recoveryHitPointsRate,  vision,  attackDamage,  attackSpeed,  attackRange,  movementSpeed,  xpOnDeath, scale)
{}


RangedEnemy::RangedEnemy(fMPoint position, Enemy* copy, ENTITY_ALIGNEMENT align) :

	Enemy(position, copy, align)
{}


RangedEnemy::~RangedEnemy()
{}