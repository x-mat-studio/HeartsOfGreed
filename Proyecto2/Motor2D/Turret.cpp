#include "Turret.h"



Turret::Turret(int turretLvl, int attackDmg, int attackSpeed, int range) :

	turretLvl(turretLvl),
	attackDmg(attackDmg),
	attackSpeed(attackSpeed),
	range(range)
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
