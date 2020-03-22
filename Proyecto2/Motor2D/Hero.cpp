#include "Hero.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "EntityManager.h"

Hero::Hero(SDL_Point position, ENTITY_TYPE type, Collider* collider,
	Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
	Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightDown,
	Animation& idleRightUp, Animation& idleLeft, Animation& idleLeftUp, Animation& idleLeftDown,
	int level, int hitPoints, int recoveryHitPointsRate, int energyPoints, int recoveryEnergyRate,
	int attackDamage, int attackSpeed, int attackRange, int movementSpeed, int vision, float attackCooldown, float skill1ExecutionTime,
	float skill2ExecutionTime, float skill3ExecutionTime, float skill1RecoverTime, float skill2RecoverTime, float skill3RecoverTime) :

	Entity(position, type, collider),

	walkLeft(walkLeft),
	walkLeftUp(walkLeftUp),
	walkLeftDown(walkLeftDown),
	walkRightUp(walkRightUp),
	walkRightDown(walkRightDown),
	walkRight(walkRight),
	idleRight(idleRight),
	idleRightDown(idleRightDown),
	idleRightUp(idleRightUp),
	idleLeft(idleLeft),
	idleLeftUp(idleLeftUp),
	idleLeftDown(idleLeftDown),

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
	skill1ExecutionTime(skill1ExecutionTime),
	skill2ExecutionTime(skill2ExecutionTime),
	skill3ExecutionTime(skill3ExecutionTime),
	skill1RecoverTime(skill1RecoverTime),
	skill2RecoverTime(skill2RecoverTime),
	skill3RecoverTime(skill3RecoverTime),

	cooldownHability1(0),
	cooldownHability2(0),
	cooldownHability3(0),
	skill1TimePassed(0),
	skill2TimePassed(0),
	skill3TimePassed(0),

	skill1Charged(true),
	skill2Charged(true),
	skill3Charged(true),
	skillFromAttacking(false),
	selected(false),

	objective(nullptr)
{}


Hero::Hero(SDL_Point position, Hero* copy) :

	Entity(position, copy->type, copy->collider),

	walkLeft(copy->walkLeft),
	walkLeftUp(copy->walkLeftUp),
	walkLeftDown(copy->walkLeftDown),
	walkRightUp(copy->walkRightUp),
	walkRightDown(copy->walkRightDown),
	walkRight(copy->walkRight),
	idleRight(copy->idleRight),
	idleRightDown(copy->idleRightDown),
	idleRightUp(copy->idleRightUp),
	idleLeft(copy->idleLeft),
	idleLeftUp(copy->idleLeftUp),
	idleLeftDown(copy->idleLeftDown),

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
	skill1ExecutionTime(copy->skill1ExecutionTime),
	skill2ExecutionTime(copy->skill2ExecutionTime),
	skill3ExecutionTime(copy->skill3ExecutionTime),
	skill1RecoverTime(copy->skill1RecoverTime),
	skill2RecoverTime(copy->skill2RecoverTime),
	skill3RecoverTime(copy->skill3RecoverTime),

	cooldownHability1(0),
	cooldownHability2(0),
	cooldownHability3(0),
	skill1TimePassed(0),
	skill2TimePassed(0),
	skill3TimePassed(0),

	skill1Charged(true),
	skill2Charged(true),
	skill3Charged(true),
	skillFromAttacking(false),
	selected(false),

	objective(nullptr)
{}


Hero::~Hero()
{
	objective = nullptr;

	walkLeft = Animation();
	walkLeftUp = Animation();
	walkLeftDown = Animation();
	walkRightUp = Animation();
	walkRightDown = Animation();
	walkRight = Animation();
	idleRight = Animation();
	idleRightDown = Animation();
	idleRightUp = Animation();
	idleLeft = Animation();
	idleLeftUp = Animation();
	idleLeftDown = Animation();
}


bool Hero::PreUpdate(float dt)
{

	return true;
}


bool Hero::Update(float dt)
{
	HERO_STATES current_state = HERO_STATES::UNKNOWN;
	//current_animation = &idle;

	//check inputs to traverse state matrix
	externalInput(inputs, dt);
	internalInput(inputs, dt);
	state = processFsm(inputs);

	if (state != current_state)
	{
		switch (state)
		{
		case HERO_STATES::IDLE:

			break;

		case HERO_STATES::MOVE:
			Move();
			break;

		case HERO_STATES::ATTACK:
			Attack();
			attackCooldown += dt;
			break;

		case HERO_STATES::CHARGING_ATTACK:
			break;

		case HERO_STATES::SKILL1:
			UseHability1();
			cooldownHability1 += dt;
			break;

		case HERO_STATES::SKILL2:
			UseHability2();
			cooldownHability2 += dt;
			break;

		case HERO_STATES::SKILL3:
			UseHability3();
			cooldownHability3 += dt;
			break;

		case HERO_STATES::REPAIR:
			break;

		case HERO_STATES::DEAD:
			Die();
			break;


		}
	}
	current_state = state;

	return true;
}


bool Hero::PostUpdate(float dt)
{
	texture;
	Draw(dt);
	return true;
}


bool Hero::MoveTo(int x, int y)
{
	//do pathfinding, if it works return true

	inputs.push_back(HERO_INPUTS::IN_MOVE);

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
}


void Hero::Draw(float dt)
{
	app->render->Blit(texture, position.x, position.y, &walkLeft.GetCurrentFrameBox(dt));
}


void Hero::Move()
{
	//Put logic to move the unit to the desired destination
	Die();
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


//Capture all the inputs, dont exactly know how, but ill figure something out
bool Hero::externalInput(std::vector<HERO_INPUTS>& inputs, float dt)
{


	return true;
}


//Here goes all timers
void Hero::internalInput(std::vector<HERO_INPUTS>& inputs, float dt)
{
	if (attackCooldown > 0)
	{
		if (attackCooldown >= attackSpeed)
		{
			inputs.push_back(HERO_INPUTS::IN_ATTACK_CHARGED);
			attackCooldown = 0;
		}

		attackCooldown += dt;
	}


	if (skill1TimePassed > 0)
	{
		if (skill1TimePassed >= skill1ExecutionTime)
		{
			inputs.push_back(HERO_INPUTS::IN_SKILL_FINISHED);
			skill1TimePassed = 0;
		}

		skill1TimePassed += dt;
	}


	if (cooldownHability1 > 0)
	{
		if (cooldownHability1 >= skill1RecoverTime)
		{
			skill1Charged = true;
		}
		
		cooldownHability1 += dt;
	}

	if (skill2TimePassed > 0)
	{
		if (skill2TimePassed >= skill2ExecutionTime)
		{
			inputs.push_back(HERO_INPUTS::IN_SKILL_FINISHED);
			skill2TimePassed = 0;
		}

		skill2TimePassed += dt;
	}

	if (cooldownHability2 > 0)
	{
		if (cooldownHability2 >= skill2RecoverTime)
		{
			skill2Charged = true;
		}

		cooldownHability2 += dt;
	}

	if (skill3TimePassed > 0)
	{
		if (skill3TimePassed >= skill3ExecutionTime)
		{
			inputs.push_back(HERO_INPUTS::IN_SKILL_FINISHED);
			skill3TimePassed = 0;
		}

		skill3TimePassed += dt;
	}

	if (cooldownHability3 > 0)
	{
		if (cooldownHability3 >= skill3RecoverTime)
		{
			skill3Charged = true;
		}

		cooldownHability3 += dt;
	}
}


HERO_STATES Hero::processFsm(std::vector<HERO_INPUTS>& inputs) {
	static HERO_STATES state = HERO_STATES::IDLE;
	HERO_INPUTS lastInput;

	while (inputs.empty() == false)
	{
		lastInput = inputs.back();
		inputs.pop_back();

		switch (state)
		{

		case HERO_STATES::IDLE:
		{
			switch (lastInput)
			{
			case HERO_INPUTS::IN_MOVE:   state = HERO_STATES::MOVE;		break;

			case HERO_INPUTS::IN_ATTACK: state = HERO_STATES::ATTACK;	break;

			case HERO_INPUTS::IN_SKILL1: state = HERO_STATES::SKILL1;	break;
			case HERO_INPUTS::IN_SKILL2: state = HERO_STATES::SKILL2;	break;
			case HERO_INPUTS::IN_SKILL3: state = HERO_STATES::SKILL3;	break;

			case HERO_INPUTS::IN_REPAIR: state = HERO_STATES::REPAIR;	break;

			case HERO_INPUTS::IN_DEAD:   state = HERO_STATES::DEAD;		break;
			}
		}	break;


		case HERO_STATES::MOVE:
		{
			switch (lastInput)
			{
			case HERO_INPUTS::IN_IDLE:   state = HERO_STATES::MOVE;		break;

			case HERO_INPUTS::IN_MOVE:   state = HERO_STATES::MOVE;		break;

			case HERO_INPUTS::IN_ATTACK: state = HERO_STATES::ATTACK;	break;

			case HERO_INPUTS::IN_SKILL1: state = HERO_STATES::SKILL1;	break;
			case HERO_INPUTS::IN_SKILL2: state = HERO_STATES::SKILL2;	break;
			case HERO_INPUTS::IN_SKILL3: state = HERO_STATES::SKILL3;	break;

			case HERO_INPUTS::IN_REPAIR: state = HERO_STATES::REPAIR;	break;

			case HERO_INPUTS::IN_DEAD:   state = HERO_STATES::DEAD;		break;
			}
		}	break;


		case HERO_STATES::ATTACK:
		{
			switch (lastInput)
			{
			case HERO_INPUTS::IN_CHARGING_ATTACK:state = HERO_STATES::CHARGING_ATTACK;			 break;

			case HERO_INPUTS::IN_OBJECTIVE_DONE: state = HERO_STATES::IDLE;					   	 break;

			case HERO_INPUTS::IN_OUT_OF_RANGE:   state = HERO_STATES::MOVE;						 break;

			case HERO_INPUTS::IN_SKILL1: state = HERO_STATES::SKILL1; skillFromAttacking = true; break;
			case HERO_INPUTS::IN_SKILL2: state = HERO_STATES::SKILL2; skillFromAttacking = true; break;
			case HERO_INPUTS::IN_SKILL3: state = HERO_STATES::SKILL3; skillFromAttacking = true; break;

			case HERO_INPUTS::IN_DEAD:   state = HERO_STATES::DEAD;								 break;
			}
		}	break;


		case HERO_STATES::CHARGING_ATTACK:
		{
			switch (lastInput)
			{
			case HERO_INPUTS::IN_ATTACK_CHARGED: state = HERO_STATES::ATTACK;					 break;

			case HERO_INPUTS::IN_OBJECTIVE_DONE: state = HERO_STATES::IDLE;						 break;

			case HERO_INPUTS::IN_SKILL1: state = HERO_STATES::SKILL1; skillFromAttacking = true; break;
			case HERO_INPUTS::IN_SKILL2: state = HERO_STATES::SKILL2; skillFromAttacking = true; break;
			case HERO_INPUTS::IN_SKILL3: state = HERO_STATES::SKILL3; skillFromAttacking = true; break;

			case HERO_INPUTS::IN_DEAD:   state = HERO_STATES::DEAD;								 break;
			}
		}	break;


		case HERO_STATES::SKILL1:
		{
			switch (lastInput)
			{
			case HERO_INPUTS::IN_SKILL_FINISHED:
			{
				if (skillFromAttacking == true)
					state = HERO_STATES::ATTACK;

				else
					state = HERO_STATES::IDLE;

				skillFromAttacking = false;
																			break;
			}

			case HERO_INPUTS::IN_OBJECTIVE_DONE: state = HERO_STATES::IDLE;	break;

			case HERO_INPUTS::IN_DEAD: state = HERO_STATES::DEAD;			break;
			}
		}	break;


		case HERO_STATES::SKILL2:
		{
			switch (lastInput)
			{
			case HERO_INPUTS::IN_SKILL_FINISHED:
			{
				if (skillFromAttacking == true)
					state = HERO_STATES::ATTACK;

				else
					state = HERO_STATES::IDLE;

				skillFromAttacking = false;
																			break;
			}

			case HERO_INPUTS::IN_OBJECTIVE_DONE: state = HERO_STATES::IDLE;	break;

			case HERO_INPUTS::IN_DEAD: state = HERO_STATES::DEAD;			break;
			}
		}	break;


		case HERO_STATES::SKILL3:
		{
			switch (lastInput)
			{
			case HERO_INPUTS::IN_SKILL_FINISHED:
			{
				if (skillFromAttacking == true)
					state = HERO_STATES::ATTACK;

				else
					state = HERO_STATES::IDLE;

				skillFromAttacking = false;
																			break;
			}

			case HERO_INPUTS::IN_OBJECTIVE_DONE: state = HERO_STATES::IDLE;	break;

			case HERO_INPUTS::IN_DEAD: state = HERO_STATES::DEAD;			break;
			}
		}	break;


		case HERO_STATES::REPAIR:
		{
			switch (lastInput)
			{
			case HERO_INPUTS::IN_ATTACKED: state = HERO_STATES::IDLE;		break;

			case HERO_INPUTS::IN_OBJECTIVE_DONE: state = HERO_STATES::IDLE;	break;

			case HERO_INPUTS::IN_DEAD: state = HERO_STATES::DEAD;			break;
			}
		}	break;


		case HERO_STATES::DEAD:
		{
		}	break;
		}

	}

	return state;
}