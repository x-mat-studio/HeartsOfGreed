#include "Hero.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"

Hero::Hero(SDL_Point position, ENTITY_TYPE type, SDL_Texture* texture, SDL_Rect collRect, COLLIDER_TYPE collType, Module* callback,
	Animation& animation, int level, int hitPoints, int recoveryHitPointsRate, int energyPoints, int recoveryEnergyRate,
	int attackDamage, int attackSpeed, int attackRange, int movementSpeed, int vision, float attackCooldown, float coolDownHability1,
	float coolDownHability2, float coolDownHability3) :

	Entity(position, type, texture, collRect, collType, callback),
	animation(animation),
	level(level),
	hitPoints(hitPoints),
	recoveryHitPointsRate(recoveryHitPointsRate),
	energyPoints(energyPoints),
	recoveryEnergyRate(recoveryEnergyRate),
	attackDamage(attackDamage),
	attackSpeed(attackSpeed),
	attackRange(attackRange),
	movementSpeed(movementSpeed),
	vision(vision),
	attackCooldown(attackCooldown),
	coolDownHability1(coolDownHability1),
	coolDownHability2(coolDownHability2),
	coolDownHability3(coolDownHability3)
{}


Hero::Hero(SDL_Point position, Hero* copy) :

	Entity(position, copy->type, copy->texture),
	animation(copy->animation),
	level(copy->level),
	hitPoints(copy->hitPoints),
	recoveryHitPointsRate(copy->recoveryHitPointsRate),
	energyPoints(copy->energyPoints),
	recoveryEnergyRate(copy->recoveryEnergyRate),
	attackDamage(copy->attackDamage),
	attackSpeed(copy->attackSpeed),
	attackRange(copy->attackRange),
	movementSpeed(copy->movementSpeed),
	vision(copy->vision),
	attackCooldown(copy->attackCooldown),
	coolDownHability1(copy->coolDownHability1),
	coolDownHability2(copy->coolDownHability2),
	coolDownHability3(copy->coolDownHability3)
{}


Hero::~Hero()
{
	objective = nullptr;

	animation = Animation();
}


bool Hero::Start()
{
	return true;
}


bool Hero::PreUpdate(float dt)
{
	return true;
}


bool Hero::Update(float dt)
{

	return true;
}


bool Hero::PostUpdate(float dt)
{
	Draw(dt);
	return true;
}


bool Hero::MoveTo(int x, int y)
{
	//do pathfinding, if it works return true
	return true;
}


bool Hero::LockOn(Entity* entity)
{
	objective = entity;
	return true;
}


void Hero::OnCollision(Collider* collider)
{

}


void Hero::LevelUp()
{
	level++;

	//Apply the necesary changes when level up

	hitPoints;
	recoveryHitPointsRate;
	energyPoints;
	recoveryEnergyRate;

	attackDamage;
	attackSpeed;
	attackRange;

	movementSpeed;
	vision;

	attackCooldown;
	coolDownHability1;
	coolDownHability2;
	coolDownHability3;
}


void Hero::Draw(float dt) 
{
	app->render->Blit(texture, position.x, position.y, &animation.GetCurrentFrameBox(dt));
}


void Hero::Move()
{
	//Put logic to move the unit to the desired destination
}


bool Hero::CheckRange(int maxDistance) 
{
	//check if the maxDistance is equal or bigger than the actual distance between the objective and the unit 
	return true;
}


void Hero::Attack()
{
	//i have to think about this one

}


void Hero::Die()
{
	toDelete = true;
}


void Hero::RecoverHealth()
{

}


void Hero::RecoverEnergy()
{

}


bool Hero::UseHability1()
{
	return true;
}


bool Hero::UseHability2()
{
	return true;
}


bool Hero::UseHability3()
{
	return true;
}