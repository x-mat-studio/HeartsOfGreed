#include "UpgradeCenter.h"



UpgradeCenter::UpgradeCenter(int turretLvl, int barricadeLvl) :
	turretLvl(turretLvl),
	barricadeLvl(barricadeLvl)
{}

UpgradeCenter::UpgradeCenter(fMPoint position, UpgradeCenter * copy, ENTITY_ALIGNEMENT alignement) : turretLvl(0), barricadeLvl(0)
{
}


UpgradeCenter::~UpgradeCenter()
{
}

bool UpgradeCenter::Start()
{
	return true;
}


bool UpgradeCenter::PreUpdate(float dt)
{
	return true;
}


bool UpgradeCenter::Update(float dt)
{
	return true;
}


bool UpgradeCenter::PostUpdate(float dt)
{
	return true;
}

void UpgradeCenter::UpgradeTurrets()
{
}

void UpgradeCenter::UpgradeBarricades()
{
}
