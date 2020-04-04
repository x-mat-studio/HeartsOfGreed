#include "Turret.h"



Turret::Turret(int turretLvl, int attackDmg, int attackSpeed, int range, fMPoint position, Collider* collider, int hitPoints, int recoveryHitPointsRate , int xpOnDeath, int buildingCost , int transparency ) :

	Building( position,  hitPoints,  recoveryHitPointsRate,  xpOnDeath,  buildingCost,  transparency,  collider),
	turretLvl(turretLvl),
	attackDmg(attackDmg),
	attackSpeed(attackSpeed),
	range(range)
{}

Turret::Turret(Turret* copy) :

	turretLvl(copy->turretLvl),
	attackDmg(copy->attackDmg),
	attackSpeed(copy->attackSpeed),
	range(copy->range),

	//Building(fMPoint position, Building* copy)
	Building(copy->position, copy) 
{}


Turret::~Turret()
{
}

bool Turret::Start()
{
	return true;
}


bool Turret::PreUpdate(float dt)
{
	return true;
}


bool Turret::Update(float dt)
{
	return true;
}


bool Turret::PostUpdate(float dt)
{
	return true;
}

void Turret::Target()
{
}

void Turret::Attack()
{
}
