#include "Hero.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "EntityManager.h"

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
	coolDownHability3(coolDownHability3),
	skillFromAttacking(false)
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
	coolDownHability3(copy->coolDownHability3),
	skillFromAttacking(false)
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
	HERO_STATES current_state = HERO_STATES::UNKNOWN;
	//current_animation = &idle;

	//check inputs to traverse state matrix
	external_input(inputs, dt);
	internal_input(inputs, dt);
	state = process_fsm(inputs);

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
			break;

		case HERO_STATES::SKILL1:
			break;

		case HERO_STATES::SKILL2:
			break;

		case HERO_STATES::SKILL3:
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
bool Hero::external_input(std::vector<HERO_INPUTS>& inputs, float dt) 
{

	
	return true;
}


//Here goes all timers
void Hero::internal_input(std::vector<HERO_INPUTS>& inputs, float dt)
{


}


HERO_STATES Hero::process_fsm(std::vector<HERO_INPUTS>& inputs) {
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
			case HERO_INPUTS::IN_OBJECTIVE_DONE: state = HERO_STATES::IDLE;					   	 break;

			case HERO_INPUTS::IN_OUT_OF_RANGE:   state = HERO_STATES::MOVE;						 break;

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
			case HERO_INPUTS::IN_ATTACKED: state = HERO_STATES::IDLE; break;

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