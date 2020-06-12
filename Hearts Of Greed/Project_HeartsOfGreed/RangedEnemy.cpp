#include "RangedEnemy.h"
#include "Render.h"
#include "EntityManager.h"
#include "Collision.h"

RangedEnemy::RangedEnemy(fMPoint position, ENTITY_TYPE type, Collider* collider, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
	Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightUp, Animation& idleRightDown, Animation& idleLeft, Animation& idleLeftUp, Animation& idleLeftDown,
	Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp, Animation& punchRightDown, Animation& punchRight,
	Animation& deathRight, Animation& deathRightUp, Animation& deathRightDown, Animation& deathLeft, Animation& deathLeftUp, Animation& deathLeftDown, Animation& particleRanged, int maxHitPoints, int currentHitPoints,
	int recoveryHitPointsRate, int vision, int attackDamage, int attackSpeed, int attackRange, int movementSpeed, int xpOnDeath, float scale) :

	Enemy(position, type, collider, walkLeft, walkLeftUp, walkLeftDown, walkRightUp,
		walkRightDown, walkRight, idleRight, idleRightUp, idleRightDown, idleLeft, idleLeftUp, idleLeftDown,
		punchLeft, punchLeftUp, punchLeftDown, punchRightUp, punchRightDown, punchRight,
		deathRight, deathRightUp, deathRightDown, deathLeft, deathLeftUp, deathLeftDown, maxHitPoints, currentHitPoints,
		recoveryHitPointsRate, vision, attackDamage, attackSpeed, attackRange, movementSpeed, xpOnDeath, scale),

	rangedAttack(particleRanged),
	projectile(nullptr),
	projectileStartedAt(0),
	projectilePos{ INT_MIN, INT_MIN }
{}


RangedEnemy::RangedEnemy(fMPoint position, Enemy* copy, ENTITY_ALIGNEMENT align) :

	Enemy(position, copy, align),
	projectile(nullptr),
	projectilePos{ INT_MIN, INT_MIN },
	projectileStartedAt(0)
{
	RangedEnemy* sampleRanged = (RangedEnemy*)copy;
	if (sampleRanged != nullptr)
		rangedAttack = (sampleRanged->rangedAttack);
	else
		rangedAttack = Animation();

	rangedAttack.loop = false;
}


RangedEnemy::~RangedEnemy()
{
	DestroyProjectile();
}


void RangedEnemy::DrawVFX(float dt)
{
	if (projectile != nullptr)
	{
		UpdateProjectile(dt);
	}
	else
	{
		LaunchProjectile();
	}

}

void RangedEnemy::LaunchProjectile()
{
	projectile = rangedAttack.GetFirstFrame();
	projectilePos = { this->position.x, this->position.y + offset.y * 0.5f };

	if (shortTermObjective != nullptr)
	{
		projectileDestination = { shortTermObjective->GetPosition().x,shortTermObjective->GetPosition().y + shortTermObjective->GetOffset().y * 0.5f };

		projectileEasingX.NewEasing(EASING_TYPE::EASE, projectilePos.x, projectileDestination.x, 0.35f);
		projectileEasingY.NewEasing(EASING_TYPE::EASE, projectilePos.y, projectileDestination.y, 0.35f);

		projectileStartedAt = SDL_GetTicks();
	}
	else
	{
		DestroyProjectile();
	}
}

void RangedEnemy::UpdateProjectile(float dt)
{
	//Move IT
	if (shortTermObjective != nullptr && (abs(projectileDestination.x - shortTermObjective->GetPosition().x) > 30 || abs(projectileDestination.y - shortTermObjective->GetPosition().y) > 30))
	{
		projectileDestination = { shortTermObjective->GetPosition().x,shortTermObjective->GetPosition().y + shortTermObjective->GetOffset().y * 0.5f };

		projectileEasingX.NewEasing(EASING_TYPE::EASE, projectilePos.x, projectileDestination.x, 0.35f - ((SDL_GetTicks() - projectileStartedAt) * 0.001f));
		projectileEasingY.NewEasing(EASING_TYPE::EASE, projectilePos.y, projectileDestination.y, 0.35f - ((SDL_GetTicks() - projectileStartedAt) * 0.001f));
	}

	if (projectileEasingX.IsActive() == true)
	{
		projectilePos.x = projectileEasingX.UpdateEasingAddingTime(dt);
		projectilePos.y = projectileEasingY.UpdateEasingAddingTime(dt);
	}

	app->render->Blit(app->entityManager->rangedW_VFX, this->projectilePos.x, this->projectilePos.y, &projectile->frame, false, true, 0, 255, 255, 255, 1.0f, projectile->pivotPositionX, projectile->pivotPositionY);


	if (abs(projectileDestination.x - projectilePos.x) < 10 && abs(projectileDestination.y - projectilePos.y) < 10)
	{
		DestroyProjectile();
	}

}

void RangedEnemy::DestroyProjectile()
{
	projectile = nullptr;
	projectilePos = { INT_MIN, INT_MIN };
	drawingVFX = false;
	projectileStartedAt = 0;
}