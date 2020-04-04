#include "Turret.h"
#include "EntityManager.h"



Turret::Turret(int turretLvl, int attackDmg, int attackSpeed, int range, fMPoint position, Collider* collider, Animation& animation, int hitPoints, int recoveryHitPointsRate, int xpOnDeath, int buildingCost, int transparency) :

	Building(position, hitPoints, recoveryHitPointsRate, xpOnDeath, buildingCost, transparency, collider),
	animation(animation),
	turretLvl(turretLvl),
	attackDmg(attackDmg),
	attackSpeed(attackSpeed),
	range(range),
	attackCD(0),
	longTermObjective{ NULL, NULL },
	shortTermObjective(nullptr),
	haveOrders(false),
	state(TURRET_STATES::IDLE)
{}


Turret::Turret(Turret* copy) :

	animation(copy->animation),
	turretLvl(copy->turretLvl),
	attackDmg(copy->attackDmg),
	attackSpeed(copy->attackSpeed),
	range(copy->range), 
	attackCD(0),
	longTermObjective{ NULL, NULL },
	shortTermObjective(nullptr),
	haveOrders(false),
	state(TURRET_STATES::IDLE),

	//Building(fMPoint position, Building* copy)
	Building(copy->position, copy) 
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
	Draw(dt);
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
	objective = app->entityManager->CheckEnemyObjectiveTurret(&rect);

	if (objective != nullptr && shortTermObjective != objective)
	{
		ret = true;
	}

	shortTermObjective = objective;

	return ret;
}


bool Turret::CheckAttackRange()
{
	if (shortTermObjective == nullptr)
	{
		return false;
	}


	fMPoint point = shortTermObjective->GetPosition();

	int distanceX = abs(position.x - point.x);
	int distanceY = abs(position.y - point.y);


	int distance = distanceX + distanceY;

	if (distance < range)
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

			case TURRET_INPUTS::IN_DEAD:   state = TURRET_STATES::DEAD;		break;
			}
		}	break;

		case TURRET_STATES::ATTACK:
		{
			switch (lastInput)
			{
			case TURRET_INPUTS::IN_CHARGING_ATTACK:		state = TURRET_STATES::CHARGING_ATTACK;	break;

			case TURRET_INPUTS::IN_DEAD:			   state = TURRET_STATES::DEAD;				break;
			}
		}	break;


		case TURRET_STATES::CHARGING_ATTACK:
		{
			switch (lastInput)
			{
			case TURRET_INPUTS::IN_ATTACK_CHARGED: state = TURRET_STATES::ATTACK;				break;

			case TURRET_INPUTS::IN_OBJECTIVE_DONE:  state = TURRET_STATES::IDLE;				break;

			case TURRET_INPUTS::IN_OUT_OF_RANGE:												break;

			case TURRET_INPUTS::IN_DEAD:			   state = TURRET_STATES::DEAD;				break;
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
			attackCD += 0.1;
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

