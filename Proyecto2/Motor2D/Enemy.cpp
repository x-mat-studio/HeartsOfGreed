#include "Enemy.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"

Enemy::Enemy(SDL_Point position, ENTITY_TYPE type, SDL_Texture* texture, SDL_Rect collRect, COLLIDER_TYPE collType, Module* callback,
	Animation& animation, int hitPoints, int recoveryHitPointsRate, int attackDamage, int attackSpeed, int attackRange, int movementSpeed,
	int xpOnDeath, float attackCooldown) :

	Entity(position, type, texture, collRect, collType, callback),
	animation(animation),
	hitPoints(hitPoints),
	recoveryHitPointsRate(recoveryHitPointsRate),
	attackDamage(attackDamage),
	attackSpeed(attackSpeed),
	attackRange(attackRange),
	movementSpeed(movementSpeed),
	xpOnDeath(xpOnDeath),
	attackCooldown(attackCooldown)
{}


Enemy::Enemy(SDL_Point position, Enemy* copy) :

	Entity(position, copy->type, copy->texture),
	animation(copy->animation),
	hitPoints(copy->hitPoints),
	recoveryHitPointsRate(copy->recoveryHitPointsRate),
	attackDamage(copy->attackDamage),
	attackSpeed(copy->attackSpeed),
	attackRange(copy->attackRange),
	movementSpeed(copy->movementSpeed),
	xpOnDeath(copy->xpOnDeath),
	attackCooldown(copy->attackCooldown)
{}


Enemy::~Enemy()
{
	objective = nullptr;

	animation = Animation();
}


bool Enemy::Start()
{
	return true;
}


bool Enemy::PreUpdate(float dt)
{
	return true;
}


bool Enemy::Update(float dt)
{

	return true;
}


bool Enemy::PostUpdate(float dt)
{
	Draw(dt);
	return true;
}


bool Enemy::MoveTo(int x, int y)
{
	//do pathfinding, if it works return true
	return true;
}


bool Enemy::LockOn(Entity* entity)
{
	objective = entity;
	return true;
}


void Enemy::OnCollision(Collider* collider)
{

}

void Enemy::Draw(float dt)
{
	app->render->Blit(texture, position.x, position.y, &animation.GetCurrentFrameBox(dt));
}


void Enemy::Move()
{
	//Put logic to move the unit to the desired destination
}

void Enemy::Attack()
{
	//i have to think about this one

}

void Enemy::Die()
{
	toDelete = true;
}

void Enemy::RecoverHealth()
{

}