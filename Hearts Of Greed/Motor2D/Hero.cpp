#include "Hero.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "FoWManager.h"
#include "Map.h"
#include "Player.h"


Hero::Hero(fMPoint position, ENTITY_TYPE type, Collider* collider,
	Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
	Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightUp,
	Animation& idleRightDown, Animation& idleLeft, Animation& idleLeftUp, Animation& idleLeftDown,
	Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp,
	Animation& punchRightDown, Animation& punchRight, Animation& skill1Right, Animation& skill1RightUp,
	Animation& skill1RightDown, Animation& skill1Left, Animation& skill1LeftUp, Animation& skill1LeftDown,
	int level, int maxHitPoints, int currentHitPoints, int recoveryHitPointsRate, int energyPoints, int recoveryEnergyRate,
	int attackDamage, float attackSpeed, int attackRange, int movementSpeed, int vision, float skill1ExecutionTime,
	float skill2ExecutionTime, float skill3ExecutionTime, float skill1RecoverTime, float skill2RecoverTime, float skill3RecoverTime,
	int skill1Dmg, SKILL_ID skill1Id, SKILL_TYPE skill1Type, ENTITY_ALIGNEMENT skill1Target) :

	DynamicEntity(position, movementSpeed, type, ENTITY_ALIGNEMENT::NEUTRAL, collider, maxHitPoints, currentHitPoints, 15, 30),

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
	punchLeft(punchLeft),
	punchLeftUp(punchLeftUp),
	punchLeftDown(punchLeftDown),
	punchRightUp(punchRightUp),
	punchRightDown(punchRightDown),
	punchRight(punchRight),
	skill1Right(skill1Right),
	skill1RightDown(skill1RightDown),
	skill1RightUp(skill1RightUp),
	skill1Left(skill1Left),
	skill1LeftUp(skill1LeftUp),
	skill1LeftDown(skill1LeftDown),

	level(level),

	recoveryHitPointsRate(recoveryHitPointsRate),
	energyPoints(energyPoints),
	recoveryEnergyRate(recoveryEnergyRate),

	attackDamage(attackDamage),
	attackSpeed(attackSpeed),
	attackRange(attackRange),
	visionDistance(vision),

	skill1ExecutionTime(skill1ExecutionTime),
	skill2ExecutionTime(skill2ExecutionTime),
	skill3ExecutionTime(skill3ExecutionTime),
	skill1RecoverTime(skill1RecoverTime),
	skill2RecoverTime(skill2RecoverTime),
	skill3RecoverTime(skill3RecoverTime),

	attackCooldown(0),
	cooldownHability1(0),
	cooldownHability2(0),
	cooldownHability3(0),
	skill1TimePassed(0),
	skill2TimePassed(0),
	skill3TimePassed(0),
	framePathfindingCount(0),
	framesPerPathfinding(FRAMES_PER_PATHFINDING),

	expToLevelUp(100),
	heroXP(0),

	skill1Charged(true),
	skill2Charged(true),
	skill3Charged(true),
	skillFromAttacking(false),
	selected(false),
	godMode(false),

	state(HERO_STATES::IDLE),
	skill1(skill1Id, skill1Dmg, skill1Type, skill1Target),
	objective(nullptr)
{
	currentAnimation = &walkLeft;

}


Hero::Hero(fMPoint position, Hero* copy, ENTITY_ALIGNEMENT alignement) :

	DynamicEntity(position, copy->unitSpeed, copy->type, alignement, copy->collider, copy->hitPointsMax, copy->hitPointsCurrent, copy->moveRange1, copy->moveRange2),

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
	punchLeft(copy->punchLeft),
	punchLeftUp(copy->punchLeftUp),
	punchLeftDown(copy->punchLeftDown),
	punchRightUp(copy->punchRightUp),
	punchRightDown(copy->punchRightDown),
	punchRight(copy->punchRight),
	skill1Right(copy->skill1Right),
	skill1RightDown(copy->skill1RightDown),
	skill1RightUp(copy->skill1RightUp),
	skill1Left(copy->skill1Left),
	skill1LeftUp(copy->skill1LeftUp),
	skill1LeftDown(copy->skill1LeftDown),

	level(copy->level),
	recoveryHitPointsRate(copy->recoveryHitPointsRate),
	energyPoints(copy->energyPoints),
	recoveryEnergyRate(copy->recoveryEnergyRate),
	attackDamage(copy->attackDamage),
	attackSpeed(copy->attackSpeed),
	attackRange(copy->attackRange),
	visionDistance(copy->visionDistance),

	skill1ExecutionTime(copy->skill1ExecutionTime),
	skill2ExecutionTime(copy->skill2ExecutionTime),
	skill3ExecutionTime(copy->skill3ExecutionTime),
	skill1RecoverTime(copy->skill1RecoverTime),
	skill2RecoverTime(copy->skill2RecoverTime),
	skill3RecoverTime(copy->skill3RecoverTime),

	attackCooldown(0),
	cooldownHability1(0),
	cooldownHability2(0),
	cooldownHability3(0),
	skill1TimePassed(0),
	skill2TimePassed(0),
	skill3TimePassed(0),
	framePathfindingCount(0),
	framesPerPathfinding(FRAMES_PER_PATHFINDING),

	expToLevelUp(100),
	heroXP(0),

	skill1Charged(true),
	skill2Charged(true),
	skill3Charged(true),
	skillFromAttacking(false),
	selected(false),
	godMode(false),

	state(HERO_STATES::IDLE),

	objective(nullptr),

	skill1(copy->skill1)
{
	currentAnimation = &walkLeft;
	//FoW Related
	visionEntity = app->fowManager->CreateFoWEntity(position, true, visionDistance);
}


Hero::~Hero()
{
	app->player->RemoveHeroFromVector(this);

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
	punchLeft = Animation();
	punchLeftUp = Animation();
	punchLeftDown = Animation();
	punchRightUp = Animation();
	punchRightDown = Animation();
	punchRight = Animation();
	skill1Right = Animation();
	skill1RightUp = Animation();
	skill1RightDown = Animation();
	skill1Left = Animation();
	skill1LeftUp = Animation();
	skill1LeftDown = Animation();

	currAoE.clear();
	suplAoE.clear();

	currAreaInfo = nullptr;
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
	GroupMovement(dt);

	CollisionPosUpdate();

	return true;
}


void Hero::StateMachine(float dt)
{
	switch (state)
	{
	case HERO_STATES::IDLE:
		break;

	case HERO_STATES::MOVE:
		if (!Move(dt))
			inputs.push_back(HERO_INPUTS::IN_IDLE);

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
				fMPoint offSet = objective->GetCenter();

				MoveTo(pos.x + offSet.x, pos.y + offSet.y);
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

			}

			else
			{
				inputs.push_back(HERO_INPUTS::IN_OUT_OF_RANGE);
			}
		}

		inputs.push_back(HERO_INPUTS::IN_CHARGING_ATTACK);
		break;

	case HERO_STATES::CHARGING_ATTACK:
		break;

	case HERO_STATES::PREPARE_SKILL1:
		PreProcessSkill1();
		break;

	case HERO_STATES::PREPARE_SKILL2:
		break;

	case HERO_STATES::PREPARE_SKILL3:
		break;

	case HERO_STATES::SKILL1:
		currentAnimation = &idleLeftDown;
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

	SetAnimation(state);
}


bool Hero::PostUpdate(float dt)
{
	if (app->debugMode)
	{
		Frame currFrame = currentAnimation->GetCurrentFrame(dt);
		DebugDraw(currFrame.pivotPositionX, currFrame.pivotPositionY);
	}

	DrawArea();

	return true;
}


bool Hero::MoveTo(int x, int y, bool haveObjective)
{
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
	if (entity == nullptr)
		return false;

	ENTITY_ALIGNEMENT align = entity->GetAlignment();

	if (align == ENTITY_ALIGNEMENT::ENEMY)
	{
		objective = entity;

		return true;
	}

	return false;
}


void Hero::OnCollision(Collider* collider)
{}


void Hero::Draw(float dt)
{
	Frame currFrame;

	if (state == HERO_STATES::CHARGING_ATTACK)
		currFrame = currentAnimation->GetCurrentFrame();
	else
		currFrame = currentAnimation->GetCurrentFrame(dt);

	app->render->Blit(texture, position.x - currFrame.pivotPositionX, position.y - currFrame.pivotPositionY, &currFrame.frame);
}

void Hero::DrawArea()
{
	if (currAreaInfo && currAoE.size() > 0)
	{
		for (int i = 0; i < currAoE.size(); i++)
		{
			iMPoint pos = app->map->MapToWorld(currAoE[i].x - 1, currAoE[i].y);
			app->render->Blit(app->entityManager->debugPathTexture, pos.x, pos.y, NULL, false, true, 100);
		}

		if (suplAoE.size() > 0)
		{
			for (int i = 0; i < suplAoE.size(); i++)
			{
				iMPoint pos = app->map->MapToWorld(suplAoE[i].x - 1, suplAoE[i].y);
				app->render->Blit(app->entityManager->debugPathTexture, pos.x, pos.y, NULL, false, true, 200);
			}
		}
	}


}

bool Hero::CheckAttackRange()
{
	//check if the maxDistance is equal or bigger than the actual distance between the objective and the unit 
	if (objective == nullptr)
	{
		return false;
	}

	if (objective->GetAlignment() == align)
	{
		objective = nullptr;
		return false;
	}

	SDL_Rect rect;
	rect.x = position.x - attackRange;
	rect.y = position.y - attackRange;
	rect.w = attackRange * 2;
	rect.h = attackRange * 2;


	if (objective->GetCollider()->CheckCollision(rect))
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
	int ret = -1;

	if (objective)
		ret = objective->RecieveDamage(attackDamage);

	if (ret > 0)
	{
		GetExperience(ret);
		true;
	}
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

	if (minimapIcon != nullptr)
	{
		minimapIcon->toDelete = true;
		minimapIcon->minimapPos = nullptr;
	}

	toDelete = true;
}


void Hero::CheckObjecive(Entity* entity)
{
	if (objective == entity)
	{
		path.clear();
		objective == nullptr;
		SearchForNewObjective();

		inputs.push_back(HERO_INPUTS::IN_MOVE);

	}
}

void Hero::SearchForNewObjective()
{
	objective = app->entityManager->SearchUnitsInRange(visionDistance, this);
}


void Hero::RecoverHealth()
{

}


void Hero::RecoverEnergy()
{

}


bool Hero::ActivateSkill1(fMPoint mouseClick)
{
	return true;
}


bool Hero::ActivateSkill2()
{
	return true;
}


bool Hero::ActivateSkill3()
{
	return true;
}


bool Hero::PrepareSkill1()
{
	if (state != HERO_STATES::SKILL1 && state != HERO_STATES::SKILL2 && state != HERO_STATES::SKILL3)
	{
		inputs.push_back(IN_PREPARE_SKILL1);
		return true;
	}

	return false;
}


bool Hero::PrepareSkill2()
{
	if (state != HERO_STATES::SKILL1 && state != HERO_STATES::SKILL2 && state != HERO_STATES::SKILL3)
	{
		inputs.push_back(IN_PREPARE_SKILL2);
		return true;
	}

	return false;
}


bool Hero::PrepareSkill3()
{
	if (state != HERO_STATES::SKILL1 && state != HERO_STATES::SKILL2 && state != HERO_STATES::SKILL3)
	{
		inputs.push_back(IN_PREPARE_SKILL3);
		return true;
	}

	return false;

}


void Hero::SkillCanceled()
{
	inputs.push_back(HERO_INPUTS::IN_SKILL_CANCEL);

}



void Hero::LevelUp()
{
	return;
}



int Hero::RecieveDamage(int damage)
{
	int ret = -1;

	if (hitPointsCurrent > 0 && godMode == false)
	{
		hitPointsCurrent -= damage;
		if (hitPointsCurrent <= 0)
		{
			Die();
			ret = 1;
		}

	}

	return ret;
}


// Returns TRUE if level up
bool Hero::GetExperience(int xp)
{
	heroXP += xp;
	return GetLevel();
}


bool Hero::GetLevel()
{
	if ((expToLevelUp * level) <= heroXP)
	{
		LevelUp();
		heroXP = 0;
		return true;
	}

	return false;
}


//Here goes all timers
void Hero::InternalInput(std::vector<HERO_INPUTS>& inputs, float dt)
{
	//TODO: This needs syncro w/ the animations
	if (attackCooldown > 0)
	{
		attackCooldown += dt;
		currentAnimation->GetCurrentFrame(attackSpeed * dt);

		if (&currentAnimation->GetCurrentFrame() == &currentAnimation->frames[currentAnimation->lastFrame - 1])
		{
			currentAnimation->ResetAnimation();

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

			case HERO_INPUTS::IN_PREPARE_SKILL1: state = HERO_STATES::PREPARE_SKILL1;  break;
			case HERO_INPUTS::IN_PREPARE_SKILL2: state = HERO_STATES::PREPARE_SKILL2;  break;
			case HERO_INPUTS::IN_PREPARE_SKILL3: state = HERO_STATES::PREPARE_SKILL3;  break;

			case HERO_INPUTS::IN_REPAIR: state = HERO_STATES::REPAIR;	break;

			case HERO_INPUTS::IN_DEAD:   state = HERO_STATES::DEAD;		break;
			}
		}	break;


		case HERO_STATES::MOVE:
		{
			switch (lastInput)
			{
			case HERO_INPUTS::IN_IDLE:   state = HERO_STATES::IDLE;		break;

			case HERO_INPUTS::IN_MOVE:   state = HERO_STATES::MOVE;		break;

			case HERO_INPUTS::IN_ATTACK: state = HERO_STATES::ATTACK;	break;

			case HERO_INPUTS::IN_PREPARE_SKILL1: state = HERO_STATES::PREPARE_SKILL1;  break;
			case HERO_INPUTS::IN_PREPARE_SKILL2: state = HERO_STATES::PREPARE_SKILL2;  break;
			case HERO_INPUTS::IN_PREPARE_SKILL3: state = HERO_STATES::PREPARE_SKILL3;  break;

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

			case HERO_INPUTS::IN_PREPARE_SKILL1: state = HERO_STATES::PREPARE_SKILL1; skillFromAttacking = true;  break;
			case HERO_INPUTS::IN_PREPARE_SKILL2: state = HERO_STATES::PREPARE_SKILL2; skillFromAttacking = true;  break;
			case HERO_INPUTS::IN_PREPARE_SKILL3: state = HERO_STATES::PREPARE_SKILL3; skillFromAttacking = true;  break;

			case HERO_INPUTS::IN_DEAD:   state = HERO_STATES::DEAD;								 break;
			}
		}	break;


		case HERO_STATES::CHARGING_ATTACK:
		{
			switch (lastInput)
			{
			case HERO_INPUTS::IN_ATTACK_CHARGED:currentAnimation->ResetAnimation();  state = HERO_STATES::ATTACK; break;

			case HERO_INPUTS::IN_OBJECTIVE_DONE: state = HERO_STATES::IDLE;	 break;

			case HERO_INPUTS::IN_OUT_OF_RANGE:    state = HERO_STATES::MOVE; break;

			case HERO_INPUTS::IN_MOVE:    state = HERO_STATES::MOVE;		break;

			case HERO_INPUTS::IN_PREPARE_SKILL1:  state = HERO_STATES::PREPARE_SKILL1; skillFromAttacking = true;  break;
			case HERO_INPUTS::IN_PREPARE_SKILL2: state = HERO_STATES::PREPARE_SKILL2; skillFromAttacking = true;  break;
			case HERO_INPUTS::IN_PREPARE_SKILL3:  state = HERO_STATES::PREPARE_SKILL3; skillFromAttacking = true;  break;

			case HERO_INPUTS::IN_DEAD:   state = HERO_STATES::DEAD;								 break;
			}
		}	break;


		case HERO_STATES::SKILL1:
		{
			switch (lastInput)
			{
			case HERO_INPUTS::IN_SKILL_FINISHED:
			{
				cooldownHability1 += TIME_TRIGGER;

				currAoE.clear();
				suplAoE.clear();
				currAreaInfo = nullptr;

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


		case HERO_STATES::PREPARE_SKILL1:
		{
			switch (lastInput)
			{
			case HERO_INPUTS::IN_SKILL_CANCEL:
			{
				currAoE.clear();
				suplAoE.clear();
				currAreaInfo = nullptr;

				if (skillFromAttacking == true)
					state = HERO_STATES::ATTACK;

				else
					state = HERO_STATES::IDLE;

				skillFromAttacking = false;
				break;
			}

			case HERO_INPUTS::IN_SKILL1:
			{
				ExecuteSkill1();
				skill1TimePassed += TIME_TRIGGER;
				state = HERO_STATES::SKILL1;
				break;
			}


			case HERO_INPUTS::IN_OBJECTIVE_DONE: skillFromAttacking = false; state = HERO_STATES::IDLE;	break;

			case HERO_INPUTS::IN_DEAD: state = HERO_STATES::DEAD;			break;
			}

		}	break;


		case HERO_STATES::PREPARE_SKILL2:
		{
			switch (lastInput)
			{
			case HERO_INPUTS::IN_SKILL_CANCEL: {

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


		case HERO_STATES::PREPARE_SKILL3:
		{
			switch (lastInput)
			{
			case HERO_INPUTS::IN_SKILL_CANCEL: {

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


void Hero::SetAnimation(HERO_STATES currState)
{

	switch (currState)
	{
	case HERO_STATES::MOVE:
	{
		switch (dir)
		{
		case FACE_DIR::NORTH_EAST:
			currentAnimation = &walkRightUp;
			break;
		case FACE_DIR::NORTH_WEST:
			currentAnimation = &walkLeftUp;
			break;
		case FACE_DIR::EAST:
			currentAnimation = &walkRight;
			break;
		case FACE_DIR::SOUTH_EAST:
			currentAnimation = &walkRightDown;
			break;
		case FACE_DIR::SOUTH_WEST:
			currentAnimation = &walkLeftDown;
			break;
		case FACE_DIR::WEST:
			currentAnimation = &walkLeft;
			break;
		}
	}
	break;
	case HERO_STATES::IDLE:
	{
		switch (dir)
		{
		case FACE_DIR::NORTH_EAST:
			currentAnimation = &idleRightUp;
			break;
		case FACE_DIR::NORTH_WEST:
			currentAnimation = &idleLeftUp;
			break;
		case FACE_DIR::EAST:
			currentAnimation = &idleRight;
			break;
		case FACE_DIR::SOUTH_EAST:
			currentAnimation = &idleRightDown;
			break;
		case FACE_DIR::SOUTH_WEST:
			currentAnimation = &idleLeftDown;
			break;
		case FACE_DIR::WEST:
			currentAnimation = &idleLeft;
			break;
		}
		break;
	}

	case HERO_STATES::CHARGING_ATTACK:
	{
		currentAnimation->loop = false;

		switch (dir)
		{

		case FACE_DIR::NORTH_EAST:
			currentAnimation = &punchRightUp;

			break;
		case FACE_DIR::NORTH_WEST:
			currentAnimation = &punchLeftUp;

			break;
		case FACE_DIR::EAST:
			currentAnimation = &punchRight;

			break;
		case FACE_DIR::SOUTH_EAST:
			currentAnimation = &punchRightDown;

			break;
		case FACE_DIR::SOUTH_WEST:
			currentAnimation = &punchLeftDown;

			break;
		case FACE_DIR::WEST:
			currentAnimation = &punchLeft;

			break;
		}
		break;

	}

	}
}

bool Hero::PreProcessSkill1()
{
	return false;
};

bool Hero::PreProcessSkill2()
{
	return false;
};

bool Hero::PreProcessSkill3()
{
	return false;
};

bool Hero::ExecuteSkill1()
{
	return app->entityManager->ExecuteSkill(skill1.dmg, this->origin, this->currAreaInfo, skill1.target, skill1.type);
};

bool Hero::ExecuteSkill2()
{
	return false;
};

bool Hero::ExecuteSkill3()
{
	return false;
};


Skill::Skill(SKILL_ID id, int dmg, SKILL_TYPE type, ENTITY_ALIGNEMENT target) : id(id), dmg(dmg), type(type), target(target)
{}

Skill::Skill(const Skill& skill1) : dmg(skill1.dmg), type(skill1.type), target(skill1.target)
{}
