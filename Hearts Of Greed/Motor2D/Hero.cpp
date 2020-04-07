#include "Hero.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "FoWManager.h"
#include "Map.h"

Hero::Hero(fMPoint position, ENTITY_TYPE type, Collider* collider,
	Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
	Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightDown,
	Animation& idleRightUp, Animation& idleLeft, Animation& idleLeftUp, Animation& idleLeftDown,
	int level, int hitPoints, int recoveryHitPointsRate, int energyPoints, int recoveryEnergyRate,
	int attackDamage, int attackSpeed, int attackRange, int movementSpeed, int vision, float attackCooldown, float skill1ExecutionTime,
	float skill2ExecutionTime, float skill3ExecutionTime, float skill1RecoverTime, float skill2RecoverTime, float skill3RecoverTime) :

	DynamicEntity(position, type, ENTITY_ALIGNEMENT::NEUTRAL, collider, 10, 20),

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
	visionDistance(visionDistance),

	attackCooldown(0),
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

	state(HERO_STATES::IDLE),

	objective(nullptr)
{
	currentAnimation = &walkLeft;

}


Hero::Hero(fMPoint position, Hero* copy, ENTITY_ALIGNEMENT alignement) :

	DynamicEntity(position, copy->type, alignement, copy->collider, copy->moveRange1, copy->moveRange2),

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
	visionDistance(copy->visionDistance),

	attackCooldown(0),
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

	state(HERO_STATES::IDLE),

	objective(nullptr)
{
	currentAnimation = &walkLeft;
	//FoW Related
	visionEntity = app->fowManager->CreateFoWEntity(position, true);
	visionEntity->SetNewVisionRadius(5);
}


Hero::~Hero()
{
	objective = nullptr;

	inputs.clear();

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

	//check inputs to traverse state matrix
	InternalInput(inputs, dt);
	state = ProcessFsm(inputs);

	StateMachine(dt);

	CollisionPosUpdate();

	return true;
}


void Hero::StateMachine(float dt)
{
	switch (state)
	{
	case HERO_STATES::IDLE:
		currentAnimation = &walkLeft;
		break;

	case HERO_STATES::MOVE:
		currentAnimation = &walkLeft;
		Move(dt * 2);
		visionEntity->SetNewPosition(position);

		if (objective != nullptr)
		{
			if (CheckAttackRange() == true)
			{
				inputs.push_back(HERO_INPUTS::IN_ATTACK);
			}

			else if (framePathfindingCount == framesPerPathfinding)
			{
				fMPoint pos = objective->GetPosition();
				MoveTo(pos.x, pos.y);
			}
		}
		
		break;

	case HERO_STATES::ATTACK:

		if (attackCooldown == 0)
		{
			if (CheckAttackRange() == true)
			{
				Attack();
				attackCooldown += TIME_TRIGGER;

				currentAnimation = &walkRight;
			}

			else
			{
				inputs.push_back(HERO_INPUTS::IN_OUT_OF_RANGE);
			}
		}

		inputs.push_back(HERO_INPUTS::IN_CHARGING_ATTACK);
		break;

	case HERO_STATES::CHARGING_ATTACK:
		currentAnimation = &idleLeftDown;
		break;

	case HERO_STATES::SKILL1:
		UseHability1();
		cooldownHability1 += TIME_TRIGGER;
		break;

	case HERO_STATES::SKILL2:
		UseHability2();
		cooldownHability2 += TIME_TRIGGER;
		break;

	case HERO_STATES::SKILL3:
		UseHability3();
		cooldownHability3 += TIME_TRIGGER;
		break;

	case HERO_STATES::REPAIR:
		break;

	case HERO_STATES::DEAD:
		Die();
		break;

	}
}


bool Hero::PostUpdate(float dt)
{
	if (app->debugMode)
		DebugDraw();

	return true;
}


bool Hero::MoveTo(int x, int y, bool haveObjective)
{
	//do pathfinding, if it works return true
	if (haveObjective == false)
	{
		objective = nullptr;
	}

	if (GeneratePath(x, y, 1))
	{
		inputs.push_back(HERO_INPUTS::IN_MOVE);
		return true;
	}

	return false;
}


bool Hero::LockOn(Entity* entity)
{
	ENTITY_ALIGNEMENT align = entity->GetAlignment();

	if (align == ENTITY_ALIGNEMENT::ENEMY)
	{
		MoveTo(entity->GetPosition().x, entity->GetPosition().y);
		objective = entity;

		return true;
	}

	else
	{
		MoveTo(entity->GetPosition().x, entity->GetPosition().y, false);
		return true;
	}

	return false;
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
	visionDistance;
}


void Hero::Draw(float dt)
{
	app->render->Blit(texture, position.x - offset.x, position.y - offset.y, &currentAnimation->GetCurrentFrameBox(dt));
}


bool Hero::CheckAttackRange()
{
	//check if the maxDistance is equal or bigger than the actual distance between the objective and the unit 
	if (objective == nullptr)
	{
		return false;
	}

	fMPoint point = objective->GetPosition();

	int distanceX = abs(position.x - point.x);
	int distanceY = abs(position.y - point.y);


	int distance = distanceX + distanceY;

	if (distance < attackRange)
	{
		return true;
	}

	else
	{
		return false;
	}
}



void Hero::Attack()
{



}


void Hero::Die()
{
	app->entityManager->AddEvent(EVENT_ENUM::ENTITY_DEAD);

	switch (type)
	{
	case ENTITY_TYPE::HERO_MELEE:
		app->eventManager->GenerateEvent(EVENT_ENUM::HERO_MELEE_OUT, EVENT_ENUM::NULL_EVENT);
		break;
	case ENTITY_TYPE::HERO_RANGED:
		app->eventManager->GenerateEvent(EVENT_ENUM::HERO_RANGED_OUT, EVENT_ENUM::NULL_EVENT);
		break;
	case ENTITY_TYPE::HERO_GATHERER:
		app->eventManager->GenerateEvent(EVENT_ENUM::HERO_GATHERER_OUT, EVENT_ENUM::NULL_EVENT);
		break;
	}

	toDelete = true;
}


void Hero::CheckObjecive(Entity* entity)
{
	if (objective == entity)
	{
		objective == nullptr;
	}
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


//Here goes all timers
void Hero::InternalInput(std::vector<HERO_INPUTS>& inputs, float dt)
{
	if (attackCooldown > 0)
	{
		attackCooldown += dt;

		if (attackCooldown >= attackSpeed * 10)
		{
			inputs.push_back(HERO_INPUTS::IN_ATTACK_CHARGED);
			attackCooldown = 0;
		}
	}


	if (skill1TimePassed > 0)
	{
		skill1TimePassed += dt;

		if (skill1TimePassed >= skill1ExecutionTime)
		{
			inputs.push_back(HERO_INPUTS::IN_SKILL_FINISHED);
			skill1TimePassed = 0;
		}
	}


	if (cooldownHability1 > 0)
	{
		cooldownHability1 += dt;

		if (cooldownHability1 >= skill1RecoverTime)
		{
			skill1Charged = true;
		}
	}

	if (skill2TimePassed > 0)
	{
		skill2TimePassed += dt;

		if (skill2TimePassed >= skill2ExecutionTime)
		{
			inputs.push_back(HERO_INPUTS::IN_SKILL_FINISHED);
			skill2TimePassed = 0;
		}
	}

	if (cooldownHability2 > 0)
	{
		cooldownHability2 += dt;

		if (cooldownHability2 >= skill2RecoverTime)
		{
			skill2Charged = true;
		}
	}

	if (skill3TimePassed > 0)
	{
		skill3TimePassed += dt;

		if (skill3TimePassed >= skill3ExecutionTime)
		{
			inputs.push_back(HERO_INPUTS::IN_SKILL_FINISHED);
			skill3TimePassed = 0;
		}
	}

	if (cooldownHability3 > 0)
	{
		cooldownHability3 += dt;

		if (cooldownHability3 >= skill3RecoverTime)
		{
			skill3Charged = true;
		}
	}


	if (framePathfindingCount < framesPerPathfinding)
	{
		framePathfindingCount++;
	}
}


HERO_STATES Hero::ProcessFsm(std::vector<HERO_INPUTS>& inputs) 
{
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

			case HERO_INPUTS::IN_SKILL1: if (skill1Charged) { state = HERO_STATES::SKILL1; } break;
			case HERO_INPUTS::IN_SKILL2: if (skill2Charged) { state = HERO_STATES::SKILL2; } break;
			case HERO_INPUTS::IN_SKILL3: if (skill3Charged) { state = HERO_STATES::SKILL3; } break;

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

			case HERO_INPUTS::IN_SKILL1: if (skill1Charged) { state = HERO_STATES::SKILL1; } break;
			case HERO_INPUTS::IN_SKILL2: if (skill2Charged) { state = HERO_STATES::SKILL2; } break;
			case HERO_INPUTS::IN_SKILL3: if (skill3Charged) { state = HERO_STATES::SKILL3; } break;

			case HERO_INPUTS::IN_REPAIR: state = HERO_STATES::REPAIR;	break;

			case HERO_INPUTS::IN_DEAD:   state = HERO_STATES::DEAD;		break;
			}
		}	break;


		case HERO_STATES::ATTACK:
		{
			switch (lastInput)
			{
			case HERO_INPUTS::IN_CHARGING_ATTACK:state = HERO_STATES::CHARGING_ATTACK;			 break;

			case HERO_INPUTS::IN_MOVE:   state = HERO_STATES::MOVE;								 break;

			case HERO_INPUTS::IN_OBJECTIVE_DONE: state = HERO_STATES::IDLE;					   	 break;

			case HERO_INPUTS::IN_OUT_OF_RANGE:   state = HERO_STATES::MOVE;						 break;

			case HERO_INPUTS::IN_SKILL1: if (skill1Charged) { state = HERO_STATES::SKILL1; skillFromAttacking = true; } break;
			case HERO_INPUTS::IN_SKILL2: if (skill2Charged) { state = HERO_STATES::SKILL2; skillFromAttacking = true; } break;
			case HERO_INPUTS::IN_SKILL3: if (skill3Charged) { state = HERO_STATES::SKILL3; skillFromAttacking = true; } break;

			case HERO_INPUTS::IN_DEAD:   state = HERO_STATES::DEAD;								 break;
			}
		}	break;


		case HERO_STATES::CHARGING_ATTACK:
		{
			switch (lastInput)
			{
			case HERO_INPUTS::IN_ATTACK_CHARGED: state = HERO_STATES::ATTACK;					 break;

			case HERO_INPUTS::IN_OBJECTIVE_DONE: state = HERO_STATES::IDLE;						 break;

			case HERO_INPUTS::IN_OUT_OF_RANGE:   state = HERO_STATES::MOVE;						 break;

			case HERO_INPUTS::IN_MOVE:			 state = HERO_STATES::MOVE;						 break;

			case HERO_INPUTS::IN_SKILL1: if (skill1Charged) { state = HERO_STATES::SKILL1; skillFromAttacking = true; } break;
			case HERO_INPUTS::IN_SKILL2: if (skill2Charged) { state = HERO_STATES::SKILL2; skillFromAttacking = true; } break;
			case HERO_INPUTS::IN_SKILL3: if (skill3Charged) { state = HERO_STATES::SKILL3; skillFromAttacking = true; } break;

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

			case HERO_INPUTS::IN_OBJECTIVE_DONE: skillFromAttacking = false; state = HERO_STATES::IDLE;	break;

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

			case HERO_INPUTS::IN_OBJECTIVE_DONE: skillFromAttacking = false; state = HERO_STATES::IDLE;	break;

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

			case HERO_INPUTS::IN_OBJECTIVE_DONE: skillFromAttacking = false; state = HERO_STATES::IDLE;	break;

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