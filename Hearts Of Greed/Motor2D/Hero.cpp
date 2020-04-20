#include "Hero.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "FoWManager.h"
#include "Map.h"
#include "Player.h"
#include "Input.h"
#include "Brofiler/Brofiler/Brofiler.h"


Hero::Hero(fMPoint position, ENTITY_TYPE type, Collider* collider,
	Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
	Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightUp,
	Animation& idleRightDown, Animation& idleLeft, Animation& idleLeftUp, Animation& idleLeftDown,
	Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp,
	Animation& punchRightDown, Animation& punchRight, Animation& skill1Right, Animation& skill1RightUp,
	Animation& skill1RightDown, Animation& skill1Left, Animation& skill1LeftUp, Animation& skill1LeftDown,
	int level, int maxHitPoints, int currentHitPoints, int recoveryHitPointsRate, int maxEnergyPoints, int energyPoints, int recoveryEnergyRate,
	int attackDamage, float attackSpeed, int attackRange, int movementSpeed, int vision, float skill1ExecutionTime,
	float skill2ExecutionTime, float skill3ExecutionTime, float skill1RecoverTime, float skill2RecoverTime, float skill3RecoverTime,
	int skill1Dmg, SKILL_ID skill1Id, SKILL_TYPE skill1Type, ENTITY_ALIGNEMENT skill1Target) :

	DynamicEntity(position, movementSpeed, type, ENTITY_ALIGNEMENT::NEUTRAL, collider, maxHitPoints, currentHitPoints, 25, 40),

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
	maxEnergyPoints(maxEnergyPoints),
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
	damageTakenTimer(0.f),
	feelingSecure(0),
	skill1Cost(20),

	expToLevelUp(100),
	heroXP(0),
	recoveringHealth(0),
	recoveringEnergy(0),

	gettingAttacked(false),
	skill1Charged(true),
	skill2Charged(true),
	skill3Charged(true),
	skillFromAttacking(false),
	//selected(false),
	godMode(false),
	currAreaInfo(nullptr),
	skillExecutionDelay(false),
	visionInPx(0.f),

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
	maxEnergyPoints(copy->maxEnergyPoints),
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
	damageTakenTimer(0.f),
	feelingSecure(0),
	skill1Cost(20),

	expToLevelUp(100),
	heroXP(0),
	recoveringHealth(0),
	recoveringEnergy(0),

	gettingAttacked(false),
	skill1Charged(true),
	skill2Charged(true),
	skill3Charged(true),
	skillFromAttacking(false),
	godMode(false),
	skillExecutionDelay(false),
	currAreaInfo(nullptr),

	state(HERO_STATES::IDLE),

	objective(nullptr),

	skill1(copy->skill1),

	drawingVfx(false)
{
	currentAnimation = &walkLeft;

	//FoW Related
	visionEntity = app->fowManager->CreateFoWEntity(position, true, visionDistance);

	float halfH = app->map->data.tileHeight * 0.5;
	float halfW = app->map->data.tileWidth * 0.5;
	visionInPx = sqrt(halfW * halfW + halfH * halfH) * visionDistance + 0.5f * visionDistance;
}


Hero::~Hero()
{
	app->player->RemoveHeroFromVector(this);

	objective = nullptr;
	currAreaInfo = nullptr;
	currentAnimation = nullptr;

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
}


bool Hero::PreUpdate(float dt)
{
	return true;
}


bool Hero::Update(float dt)
{
	BROFILER_CATEGORY("Hero Update", Profiler::Color::Blue);

	//check inputs to traverse state matrix
	InternalInput(inputs, dt);
	state = ProcessFsm(inputs);

	StateMachine(dt);
	GroupMovement(dt);

	FeelingSecure(dt);
	if (!gettingAttacked)
	{
		RecoverHealth(dt);
		RecoverEnergy(dt);
	}


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
	{
		bool hasMoved = false;
		hasMoved = Move(dt);

		if (visionEntity != nullptr)
		{
			visionEntity->SetNewPosition(position);
		}


		if (objective != nullptr)
		{
			if (CheckAttackRange() == true)
			{
				inputs.push_back(HERO_INPUTS::IN_ATTACK);
			}

			else if (framePathfindingCount == framesPerPathfinding)
			{
				if (objective != nullptr)
				{
					fMPoint pos = objective->GetPosition();
					fMPoint offSet = objective->GetOffset();

					MoveTo(pos.x + offSet.x, pos.y + offSet.y);
				}
			}
		}
		else if (!hasMoved)
			inputs.push_back(HERO_INPUTS::IN_IDLE);

	}
	break;

	case HERO_STATES::ATTACK:

		if (attackCooldown == 0.f)
		{
			if (CheckAttackRange() == true)
			{
				Attack();
				if (objective != nullptr)
					dir = DetermineDirection(objective->position - position);

				attackCooldown += TIME_TRIGGER;

			}
			else
			{
				inputs.push_back(HERO_INPUTS::IN_OUT_OF_RANGE);
			}
		}
		else
			inputs.push_back(HERO_INPUTS::IN_CHARGING_ATTACK);

		break;

	case HERO_STATES::CHARGING_ATTACK:
		if (attackCooldown == 0.f)
			inputs.push_back(HERO_INPUTS::IN_ATTACK);

		break;

	case HERO_STATES::PREPARE_SKILL1:
		PreProcessSkill1();
		dir = DetermineDirection(app->input->GetMousePosWorld() - position);
		break;

	case HERO_STATES::PREPARE_SKILL2:
		break;

	case HERO_STATES::PREPARE_SKILL3:
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

	if (state == HERO_STATES::CHARGING_ATTACK || state == HERO_STATES::PREPARE_SKILL1)
		currFrame = currentAnimation->GetCurrentFrame();
	else
		currFrame = currentAnimation->GetCurrentFrame(dt);

	if (damageTakenTimer > 0.f)
		app->render->Blit(texture, position.x - currFrame.pivotPositionX, position.y - currFrame.pivotPositionY, &currFrame.frame, false, true, 0, 255, 0, 0);

	else
		app->render->Blit(texture, position.x - currFrame.pivotPositionX, position.y - currFrame.pivotPositionY, &currFrame.frame, false, true, 0, 255, 255, 255);

	if (drawingVfx)
		DrawVfx(dt);
}

void Hero::DrawArea()
{
	if (currAreaInfo && currAoE.size() > 0)
	{
		for (uint i = 0; i < currAoE.size(); i++)
		{
			iMPoint pos = app->map->MapToWorld(currAoE[i].x - 1, currAoE[i].y);
			app->render->Blit(app->entityManager->debugPathTexture, pos.x, pos.y, NULL, false, true, 100);
		}

		if (suplAoE.size() > 0)
		{
			for (uint i = 0; i < suplAoE.size(); i++)
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
	toDelete = true;

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

	if (visionEntity != nullptr)
	{
		visionEntity->deleteEntity = true;
		visionEntity = nullptr;
	}

	app->audio->PlayFx(app->entityManager->suitmanGetsDeath2, 0, 5, this->GetMyLoudness(), this->GetMyDirection());

}


void Hero::CheckObjective(Entity* entity)
{
	if (objective == entity)
	{
		path.clear();
		objective = nullptr;
		SearchForNewObjective();

		inputs.push_back(HERO_INPUTS::IN_MOVE);

	}
}


void Hero::SearchForNewObjective()
{
	SDL_Rect rect;

	rect.x = position.x - visionInPx;
	rect.y = position.y - visionInPx;
	rect.w = visionInPx * 2;
	rect.h = visionInPx * 2;

	objective = app->entityManager->SearchEntityRect(&rect, align);
}


void Hero::FeelingSecure(float dt)
{
	if (gettingAttacked)
	{
		feelingSecure += 1.00f * dt;

		if (feelingSecure >= 5)
		{
			gettingAttacked = false;
			feelingSecure = 0;
		}
	}
}


void Hero::PlayGenericNoise()
{
	//Herency only
}


void Hero::RecoverHealth(float dt)
{
	if (!gettingAttacked && (hitPointsMax > hitPointsCurrent))
	{
		recoveringHealth += 1.00f * dt;

		if (recoveringHealth >= 2)
		{
			hitPointsCurrent += recoveryHitPointsRate;
			recoveringHealth = 0;
		}
	}
}


void Hero::RecoverEnergy(float dt)
{
	if (!gettingAttacked && (maxEnergyPoints > energyPoints))
	{
		recoveringEnergy += 1.00f * dt;

		if (recoveringEnergy >= 2)
		{
			energyPoints += recoveryEnergyRate;
			recoveringEnergy = 0;
		}
	}
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
	gettingAttacked = true;
	feelingSecure = 0;

	if (hitPointsCurrent > 0 && godMode == false)
	{
		hitPointsCurrent -= damage;

		damageTakenTimer = 0.3f;

		if (hitPointsCurrent <= 0)
		{
			Die();
			ret = 1;
		}
		else
		{
			int randomCounter = rand() % 5;

			if (randomCounter == 0)
				app->audio->PlayFx(app->entityManager->suitmanGetsHit2, 0, 5, this->GetMyLoudness(), this->GetMyDirection(), true);
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
		app->audio->PlayFx(app->entityManager->lvlup, 0, -1, LOUDNESS::LOUD, DIRECTION::FRONT);
		level++;
		return true;
	}

	return false;
}


//Here goes all timers
void Hero::InternalInput(std::vector<HERO_INPUTS>& inputs, float dt)
{
	if (attackCooldown > 0.f)
	{
		attackCooldown += dt;

		if (state == HERO_STATES::CHARGING_ATTACK || state == HERO_STATES::ATTACK)
		{
			currentAnimation->GetCurrentFrame(attackSpeed * dt);

			if (&currentAnimation->GetCurrentFrame() >= &currentAnimation->frames[currentAnimation->lastFrame - 1])
			{
				currentAnimation->ResetAnimation();

				inputs.push_back(HERO_INPUTS::IN_ATTACK_CHARGED);
				attackCooldown = 0.f;

			}
		}
	}

	if (skill1TimePassed > 0.f)
	{
		skill1TimePassed += dt;

		if (skill1TimePassed >= skill1ExecutionTime)
		{

			inputs.push_back(HERO_INPUTS::IN_SKILL_FINISHED);

			if (skillExecutionDelay)
			{
				ExecuteSkill1();
				skillExecutionDelay = false;
			}

			skill1TimePassed = 0.f;
		}
	}


	if (cooldownHability1 > 0.f)
	{
		cooldownHability1 += dt;

		drawingVfx = true;

		if (cooldownHability1 >= skill1RecoverTime)
		{
			skill1Charged = true;
			drawingVfx = false;
			cooldownHability1 = 0.f;
		}
	}

	if (skill2TimePassed > 0.f)
	{
		skill2TimePassed += dt;

		if (skill2TimePassed >= skill2ExecutionTime)
		{
			inputs.push_back(HERO_INPUTS::IN_SKILL_FINISHED);
			skill2TimePassed = 0.f;
		}
	}

	if (cooldownHability2 > 0.f)
	{
		cooldownHability2 += dt;

		if (cooldownHability2 >= skill2RecoverTime)
		{
			skill2Charged = true;
		}
	}

	if (skill3TimePassed > 0.f)
	{
		skill3TimePassed += dt;

		if (skill3TimePassed >= skill3ExecutionTime)
		{
			inputs.push_back(HERO_INPUTS::IN_SKILL_FINISHED);
			skill3TimePassed = 0.f;
		}
	}

	if (cooldownHability3 > 0.f)
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

	if (damageTakenTimer > 0.f)
	{
		damageTakenTimer -= dt;

		if (damageTakenTimer <= 0.f)
			damageTakenTimer = 0.f;
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
			case HERO_INPUTS::IN_MOVE:   state = HERO_STATES::MOVE;		PlayGenericNoise(); break;

			case HERO_INPUTS::IN_ATTACK:
				attackCooldown += TIME_TRIGGER;
				state = HERO_STATES::ATTACK;	PlayGenericNoise(); break;

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

			case HERO_INPUTS::IN_ATTACK:
				PlayGenericNoise();
				attackCooldown += TIME_TRIGGER;
				state = HERO_STATES::ATTACK;	break;

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

			case HERO_INPUTS::IN_MOVE:  PlayGenericNoise(); state = HERO_STATES::MOVE;								 break;

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
				currentAnimation->ResetAnimation();

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
				skill1Charged = false;
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
		currentAnimation->loop = false;

		break;
	}

	case HERO_STATES::PREPARE_SKILL1:
	{
		switch (dir)
		{
		case FACE_DIR::NORTH_EAST:
			currentAnimation = &skill1RightUp;
			break;

		case FACE_DIR::NORTH_WEST:
			currentAnimation = &skill1LeftUp;
			break;

		case FACE_DIR::EAST:
			currentAnimation = &skill1Right;
			break;

		case FACE_DIR::SOUTH_EAST:
			currentAnimation = &skill1RightDown;
			break;

		case FACE_DIR::SOUTH_WEST:
			currentAnimation = &skill1LeftDown;
			break;

		case FACE_DIR::WEST:
			currentAnimation = &skill1Left;
			break;
		}
		if(currentAnimation->GetCurrentFrameNum() > 0)
			currentAnimation->ResetAnimation();

		currentAnimation->loop = false;

		break;
	}

	case HERO_STATES::SKILL1:
	{

		switch (dir)
		{
		case FACE_DIR::NORTH_EAST:
			currentAnimation = &skill1RightUp;
			break;

		case FACE_DIR::NORTH_WEST:
			currentAnimation = &skill1LeftUp;
			break;

		case FACE_DIR::EAST:
			currentAnimation = &skill1Right;
			break;

		case FACE_DIR::SOUTH_EAST:
			currentAnimation = &skill1RightDown;
			break;

		case FACE_DIR::SOUTH_WEST:
			currentAnimation = &skill1LeftDown;
			break;

		case FACE_DIR::WEST:
			currentAnimation = &skill1Left;
			break;
		}
		break;

		currentAnimation->loop = false;

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
	int ret = 0;

	ret = app->entityManager->ExecuteSkill(skill1.dmg, this->origin, this->currAreaInfo, skill1.target, skill1.type);

	if (ret > 0)
	{
		GetExperience(ret);
	}
	return true;
};

bool Hero::ExecuteSkill2()
{
	return false;
};

bool Hero::ExecuteSkill3()
{
	return false;
}

void Hero::DrawSelected()
{
	if (selectedByPlayer == true)
		app->render->Blit(app->entityManager->selectedTexture, this->collider->rect.x + this->collider->rect.w / 2, this->collider->rect.y);
}

bool Hero::DrawVfx(float dt)
{
	return false;
}


Skill::Skill(SKILL_ID id, int dmg, SKILL_TYPE type, ENTITY_ALIGNEMENT target) : id(id), dmg(dmg), type(type), target(target)
{}

Skill::Skill(const Skill& skill1) : dmg(skill1.dmg), type(skill1.type), target(skill1.target), id(skill1.id)
{}
