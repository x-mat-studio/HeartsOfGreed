#include "Turret.h"
#include "EntityManager.h"
#include "Render.h"



Turret::Turret(int turretLvl, int attackDmg, int attackSpeed, int range, fMPoint position, Collider* collider, Animation& animation, int maxHitPoints, int currentHitPoints, int recoveryHitPointsRate, int xpOnDeath, int buildingCost, int transparency) :

	Building(position, maxHitPoints, currentHitPoints, recoveryHitPointsRate, xpOnDeath, buildingCost, transparency, collider, ENTITY_TYPE::BLDG_TURRET),
	animation(animation),
	turretLvl(turretLvl),
	attackDmg(attackDmg),
	attackSpeed(attackSpeed),
	range(range),

	attackCD(0),
	
	shortTermObjective(nullptr),
	haveOrders(false),

	state(TURRET_STATES::IDLE)
{}


Turret::Turret(fMPoint position, Turret* copy, ENTITY_ALIGNEMENT alignement) :

	Building(position, copy, alignement),

	animation(copy->animation),
	turretLvl(copy->turretLvl),
	attackDmg(copy->attackDmg),
	attackSpeed(copy->attackSpeed),
	range(copy->range), 

	attackCD(0),

	shortTermObjective(nullptr),
	haveOrders(false),

	state(TURRET_STATES::IDLE)
{}


Turret::~Turret()
{
	shortTermObjective = nullptr;

	inputs.clear();

	animation = Animation();
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


	//check inputs to traverse state matrix
	ExternalInput(inputs, dt);
	InternalInput(inputs, dt);
	state = ProcessFsm(inputs);


	StateMachine();

	return true;
}


bool Turret::PostUpdate(float dt)
{

	return true;
}


void Turret::CheckObjecive(Entity* entity)
{
	if (shortTermObjective == entity)
	{
		shortTermObjective == nullptr;
	}
}


bool Turret::SearchObjective()
{
	bool ret = false;

	SDL_Rect rect;

	rect.x = position.x - range;
	rect.y = position.y - range;
	rect.w = range * 2;
	rect.h = range * 2;

	Entity* objective;
	objective = app->entityManager->SearchEntityRect(&rect, align);

	if (objective != nullptr && shortTermObjective != objective)
	{
		ret = true;
	}

	shortTermObjective = objective;

	return ret;
}

void Turret::Draw(float dt)
{
	app->render->Blit(texture, position.x, position.y, &animation.GetCurrentFrameBox(dt));
}

int Turret::GetLvl()
{
	return turretLvl;
}

int Turret::GetAD()
{
	return attackDmg;
}

int Turret::GetAS()
{
	return attackSpeed;
}

int Turret::GetRng()
{
	return range;
}


bool Turret::CheckAttackRange()
{
	if (shortTermObjective == nullptr)
	{
		return false;
	}


	fMPoint point = shortTermObjective->GetPosition();

	if (point.DistanceManhattan(position) < range)
	{
		return true;
	}
	else
	{
		inputs.push_back(TURRET_INPUTS::IN_OUT_OF_RANGE);
		return false;
	}
}


void Turret::Attack()
{
	LOG("The turret goes brbrbr");
}


void Turret::Die()
{
	app->entityManager->AddEvent(EVENT_ENUM::ENTITY_DEAD);
	toDelete = true;

	if (minimapIcon != nullptr)
	{
		minimapIcon->toDelete = true;
		minimapIcon->minimapPos = nullptr;
	}
}


Entity* Turret::EnemyInRange()
{


	return nullptr;
}


void Turret::InternalInput(std::vector<TURRET_INPUTS>& inputs, float dt)
{
	if (attackCD > 0)
	{
		attackCD += dt;

		if (attackCD >= attackSpeed)
		{
			inputs.push_back(TURRET_INPUTS::IN_ATTACK_CHARGED);
			attackCD = 0;
		}
	}
}


bool Turret::ExternalInput(std::vector<TURRET_INPUTS>& inputs, float dt)
{
	if (CheckAttackRange())
	{
		inputs.push_back(TURRET_INPUTS::IN_ATTACK);
	}
	else 
	{
		SearchObjective();
	}

	return true;
}


TURRET_STATES Turret::ProcessFsm(std::vector<TURRET_INPUTS>& inputs)
{
	TURRET_INPUTS lastInput;

	while (inputs.empty() == false)
	{
		lastInput = inputs.back();
		inputs.pop_back();

		switch (state)
		{

		case TURRET_STATES::IDLE:
		{
			switch (lastInput)
			{
			case TURRET_INPUTS::IN_ATTACK: state = TURRET_STATES::ATTACK;		break;

			case TURRET_INPUTS::IN_DEAD:   state = TURRET_STATES::DEAD;			break;
			}
		}	break;

		case TURRET_STATES::ATTACK:
		{
			switch (lastInput)
			{
			case TURRET_INPUTS::IN_CHARGING_ATTACK:		state = TURRET_STATES::CHARGING_ATTACK;	break;

			case TURRET_INPUTS::IN_DEAD:			    state = TURRET_STATES::DEAD;			break;
			}
		}	break;


		case TURRET_STATES::CHARGING_ATTACK:
		{
			switch (lastInput)
			{
			case TURRET_INPUTS::IN_ATTACK_CHARGED:  state = TURRET_STATES::ATTACK;				break;

			case TURRET_INPUTS::IN_OBJECTIVE_DONE:  state = TURRET_STATES::IDLE;				break;

			case TURRET_INPUTS::IN_OUT_OF_RANGE:	state = TURRET_STATES::IDLE;				break;

			case TURRET_INPUTS::IN_DEAD:			state = TURRET_STATES::DEAD;				break;
			}
		}	break;


		case TURRET_STATES::DEAD:
		{
		}	break;

		}

	}

	return state;
}


void Turret::StateMachine()
{
	switch (state)
	{
	case TURRET_STATES::IDLE:

		break;

	case TURRET_STATES::ATTACK:

		if (attackCD == 0)
		{
			Attack();
			attackCD += 0.001;
		}

		inputs.push_back(TURRET_INPUTS::IN_CHARGING_ATTACK);
		break;

	case TURRET_STATES::CHARGING_ATTACK:
		break;

	case TURRET_STATES::DEAD:
		Die();
		break;
	}
}

