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
#include "Pathfinding.h"
#include "Brofiler/Brofiler/Brofiler.h"


Hero::Hero(fMPoint position, ENTITY_TYPE type, Collider* col,
	Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
	Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightUp,
	Animation& idleRightDown, Animation& idleLeft, Animation& idleLeftUp, Animation& idleLeftDown,
	Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp,
	Animation& punchRightDown, Animation& punchRight, Animation& skill1Right, Animation& skill1RightUp,
	Animation& skill1RightDown, Animation& skill1Left, Animation& skill1LeftUp, Animation& skill1LeftDown,
	Animation& deathRight, Animation& deathRightUp, Animation& deathRightDown, Animation& deathLeft, Animation& deathLeftUp, Animation& deathLeftDown, Animation& tileOnWalk,
	HeroStats& stats, Skill& skill1) :

	DynamicEntity(position, stats.movSpeed, type, ENTITY_ALIGNEMENT::NEUTRAL, col, stats.maxHP, stats.maxHP, 25, 40),

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
	deathRight(deathRight),
	deathRightDown(deathRightDown),
	deathRightUp(deathRightUp),
	deathLeft(deathLeft),
	deathLeftUp(deathLeftUp),
	deathLeftDown(deathLeftDown),

	tileOnWalk(tileOnWalk),

	stats(stats),

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

	heroXP(0),
	recoveringHealth(0),
	recoveringEnergy(0),

	gettingAttacked(false),
	skill1Charged(true),
	skill2Charged(true),
	skill3Charged(true),
	skillFromAttacking(false),
	godMode(false),
	currAreaInfo(nullptr),
	skillExecutionDelay(false),
	visionInPx(0.f),
	movingTo{ -1,-1 },
	drawingVfx(false),
	lvlUpSfxTimer(0),

	state(HERO_STATES::IDLE),
	skill1(skill1),
	objective(nullptr),
	myParticleSystem(nullptr),


	heroSkillPoints(0),
	comeFromAttack(true)

{
	currentAnimation = &walkLeft; 
}


Hero::Hero(fMPoint position, Hero* copy, ENTITY_ALIGNEMENT alignement) :

	DynamicEntity(position, copy->unitSpeed, copy->type, alignement, copy->collider, copy->stats.maxHP, copy->stats.maxHP, copy->moveRange1, copy->moveRange2),

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
	deathRight(copy->deathRight),
	deathRightDown(copy->deathRightDown),
	deathRightUp(copy->deathRightUp),
	deathLeft(copy->deathLeft),
	deathLeftUp(copy->deathLeftUp),
	deathLeftDown(copy->deathLeftDown),

	tileOnWalk(copy->tileOnWalk),

	stats(copy->stats),

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


	heroSkillPoints(0),

	drawingVfx(false)
{
	currentAnimation = &walkLeft;
	tileOnWalk.loop = false;

	//FoW Related
	visionEntity = app->fowManager->CreateFoWEntity(position, true, stats.visionDistance);

	float halfH = app->map->data.tileHeight * 0.5;
	float halfW = app->map->data.tileWidth * 0.5;
	visionInPx = sqrt(halfW * halfW + halfH * halfH) * stats.visionDistance + 0.5f * stats.visionDistance;
}


Hero::~Hero()
{
	app->player->RemoveHeroFromVector(this);

	objective = nullptr;
	currAreaInfo = nullptr;
	currentAnimation = nullptr;
	myParticleSystem = nullptr;

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
	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_STATE::KEY_DOWN)
	{
		LevelUp();
	}
	return true;
}


bool Hero::Update(float dt)
{
	BROFILER_CATEGORY("Hero Update", Profiler::Color::Blue);

	//check inputs to traverse state matrix
	InternalInput(inputs, dt);
	state = ProcessFsm(inputs);

	StateMachine(dt);
	UpdatePasiveSkill(dt);

	GroupMovement(dt);

	FeelingSecure(dt);

	if (!gettingAttacked)
	{
		RecoverHealth(dt);
		RecoverEnergy(dt);
	}

	CollisionPosUpdate();
	HandleMyParticleSystem(dt);
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

					MoveTo(pos.x, pos.y);


				}
				framePathfindingCount = 0;
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
				if (currentAnimation->GetCurrentFrameNum() >= currentAnimation->lastFrame * 0.5f)
				{
					Attack();
					if (objective != nullptr)
						dir = DetermineDirection(objective->position - position);

					attackCooldown += TIME_TRIGGER;
				}

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
			inputs.push_back(HERO_INPUTS::IN_ATTACK_CHARGED);

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
		Frame currFrame = currentAnimation->GetCurrentFrame();
		DebugDraw();
	}

	DrawArea();

	CommandVfx(dt);

	

	return true;
}

void Hero::CommandVfx(float dt)
{
	if (path.size() > 0)
	{
		Frame currFrame = tileOnWalk.GetCurrentFrame(dt);

		int drawAlpha = 125;

		if (selectedByPlayer)
			drawAlpha = 225;

		BlitCommandVfx(currFrame, drawAlpha);
	}
	else
	{
		movingTo = { -1, -1 };
		//tileOnWalk.ResetAnimation();
	}


}

bool Hero::MoveTo(int x, int y, bool haveObjective)
{


	if (haveObjective == false)
	{
		objective = nullptr;
	}

	if (GeneratePath(x, y, 1))
	{

		movingTo = app->pathfinding->GetDestination(this);

		if (movingTo.x == INT_MIN && !path.empty())
			movingTo = path.back();

		movingTo = app->map->MapToWorld(movingTo.x, movingTo.y);

		tileOnWalk.ResetAnimation();

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
	Frame currFrame = GetAnimationCurrentFrame(dt);

	if (damageTakenTimer > 0.f)
		app->render->Blit(texture, position.x, position.y, &currFrame.frame, false, true, 0, 255, 0, 0, 0.75f, currFrame.pivotPositionX, currFrame.pivotPositionY);

	else
		app->render->Blit(texture, position.x, position.y, &currFrame.frame, false, true, 0, 255, 255, 255, 0.75f, currFrame.pivotPositionX, currFrame.pivotPositionY);

	if (drawingVfx)
		DrawVfx(dt);
}

Frame Hero::GetAnimationCurrentFrame(float dt)
{
	Frame currFrame;

	if (state == HERO_STATES::ATTACK || state == HERO_STATES::CHARGING_ATTACK)
	{
		currFrame = currentAnimation->GetCurrentFrame(dt * stats.atkSpeed);
	}
	else if (state == HERO_STATES::PREPARE_SKILL1 || (state == HERO_STATES::CHARGING_ATTACK && !comeFromAttack))
	{
		currFrame = currentAnimation->GetCurrentFrame();
	}
	else
		currFrame = currentAnimation->GetCurrentFrame(dt);

	return currFrame;
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


void Hero::UpdatePasiveSkill(float dt)
{
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


	iMPoint myPos = app->map->WorldToMap(position.x, position.y);

	fMPoint objPosW = objective->GetPosition();
	iMPoint objPosM = app->map->WorldToMap(objPosW.x, objPosW.y);


	if (app->pathfinding->CreateLine(myPos, objPosM).size()-1 < stats.attackRange + objective->GetRadiusSize())
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
		ret = objective->RecieveDamage(stats.damage);

	if (ret > 0)
	{
		GetExperience(ret);

		if (this->type == ENTITY_TYPE::HERO_GATHERER && app->player != nullptr) {
			app->player->AddResources(ret * 0.5f);

		}
		true;
	}
}


void Hero::Die()
{
	toDelete = true;

	app->eventManager->GenerateEvent(EVENT_ENUM::ENTITY_DEAD, EVENT_ENUM::NULL_EVENT);

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
	app->entityManager->AssignNewDeadHero(*this);

	if (myParticleSystem != nullptr)
	{
		myParticleSystem->Die();
	}
	
}

void Hero::ExecuteDyingFX(int sfx)
{
	app->audio->PlayFx(sfx, 0, -1, this->GetMyLoudness(), this->GetMyDirection());
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


void Hero::PlayGenericNoise(int random)
{
	//Herency only
	return;
}

void Hero::BlitCommandVfx(Frame& currframe, int alphaValue)
{

	return;
}


void Hero::RecoverHealth(float dt)
{
	if (!gettingAttacked && (stats.maxHP > stats.currHP))
	{
		recoveringHealth += 1.00f * dt;

		if (recoveringHealth >= 2)
		{
			stats.currHP += stats.recoveryHPRate;
			recoveringHealth = 0;
		}
	}
}


void Hero::RecoverEnergy(float dt)
{
	if (!gettingAttacked && (stats.maxEnergy > stats.currEnergy))
	{
		recoveringEnergy += 1.00f * dt;

		if (recoveringEnergy >= 2)
		{
			stats.currEnergy += stats.recoveryEnergyRate;
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

	if (stats.currHP > 0 && godMode == false)
	{
		stats.currHP -= damage;

		damageTakenTimer = 0.3f;

		if (stats.currHP <= 0)
		{
			Die();
			ret = 1;
		}
		else
		{
			int randomCounter = rand() % 5;

			if (randomCounter == 0)
			{
				PlayOnHitSound();
			}
		}
	}

	return ret;
}

void Hero::PlayOnHitSound()
{
	app->audio->PlayFx(app->entityManager->suitmanGetsHit2, 0, -1, this->GetMyLoudness(), this->GetMyDirection(), true);

}


// Returns TRUE if level up
bool Hero::GetExperience(int xp)
{
	heroXP += xp;
	app->player->AddResourcesBoost(1);
	return GetLevel();
}


bool Hero::GetLevel()
{
	if ((stats.xpToLvlUp * stats.heroLevel) <= heroXP)
	{
		LevelUp();
		heroXP = 0;
		app->audio->PlayFx(app->entityManager->lvlup, 0, -1, LOUDNESS::LOUD, DIRECTION::FRONT);
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

		if (attackCooldown > (1 / stats.atkSpeed))
		{
			inputs.push_back(HERO_INPUTS::IN_ATTACK_CHARGED);
			attackCooldown = 0.f;
			comeFromAttack = true;

			ResetAttackAnimation();
		}

	}

	if (skill1TimePassed > 0.f)
	{
		skill1TimePassed += dt;

		if (skill1TimePassed >= skill1.executionTime)
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

		if (cooldownHability1 >= skill1.coolDown || godMode)
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
			case HERO_INPUTS::IN_MOVE:   state = HERO_STATES::MOVE;		PlayGenericNoise(33); break;

			case HERO_INPUTS::IN_ATTACK:
				attackCooldown += TIME_TRIGGER; comeFromAttack = false;
				state = HERO_STATES::ATTACK;	PlayGenericNoise(33); break;

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
				PlayGenericNoise(33);
				attackCooldown += TIME_TRIGGER;
				comeFromAttack = false;
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

			case HERO_INPUTS::IN_MOVE:  PlayGenericNoise(33); state = HERO_STATES::MOVE;		break;

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
			case HERO_INPUTS::IN_ATTACK_CHARGED: state = HERO_STATES::ATTACK; break;

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
	case HERO_STATES::ATTACK:
	{

		if (!comeFromAttack)
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

		if (currentAnimation->GetCurrentFrameNum() > 0)
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

void Hero::HandleMyParticleSystem(float dt)
{
	if (myParticleSystem != nullptr) {
	
		myParticleSystem->Move(position.x, position.y);
	
		if (myParticleSystem->IsActive()) {
		
			TimeMyParticleSystem(dt);
		}
	}
}

void Hero::ResetAttackAnimation()
{
	punchRightUp.ResetAnimation();
	punchLeftUp.ResetAnimation();
	punchRight.ResetAnimation();
	punchRightDown.ResetAnimation();
	punchLeftDown.ResetAnimation();
	punchLeft.ResetAnimation();
}

void Hero::TimeMyParticleSystem(float dt)
{
	//implied that your system is not nullptr
	if (myParticleSystem->IsActive()) {
		lvlUpSfxTimer += dt;

		if (lvlUpSfxTimer > 3) {
			lvlUpSfxTimer = 0;
			myParticleSystem->Desactivate();
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

	ret = app->entityManager->ExecuteSkill(skill1, this->origin);

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


Skill::Skill() : id(SKILL_ID::NO_TYPE), dmg(-1), coolDown(-1.f), rangeRadius(-1), attackRadius(-1), hurtYourself(false), type(SKILL_TYPE::NO_TYPE), target(ENTITY_ALIGNEMENT::UNKNOWN), effect(SKILL_EFFECT::NO_EFFECT), executionTime(-1.f), lvl(-1), energyCost(-1)
{
}

Skill::Skill(SKILL_ID id, int dmg, int cooldown, int rangeRadius, int attackRadius, bool hurtYourself, float executionTime, SKILL_TYPE type, ENTITY_ALIGNEMENT target, int lvl, int energyCost, SKILL_EFFECT effect) :
	id(id), dmg(dmg), coolDown(cooldown), rangeRadius(rangeRadius), attackRadius(attackRadius), hurtYourself(hurtYourself), type(type), target(target), effect(effect), executionTime(executionTime), lvl(lvl), energyCost(energyCost)
{}

Skill::Skill(const Skill& skill1) : dmg(skill1.dmg), type(skill1.type), target(skill1.target), id(skill1.id), effect(skill1.effect), coolDown(skill1.coolDown), attackRadius(skill1.attackRadius), rangeRadius(skill1.rangeRadius), hurtYourself(skill1.hurtYourself), executionTime(skill1.executionTime), lvl(skill1.lvl), energyCost(skill1.energyCost)
{}

Skill Skill::operator=(Skill& newSkill)
{
	this->attackRadius = newSkill.attackRadius;
	this->coolDown = newSkill.coolDown;
	this->dmg = newSkill.dmg;
	this->effect = newSkill.effect;
	this->executionTime = newSkill.executionTime;
	this->hurtYourself = newSkill.hurtYourself;
	this->id = newSkill.id;
	this->rangeRadius = newSkill.rangeRadius;
	this->target = newSkill.target;
	this->type = newSkill.type;

	this->lvl = newSkill.lvl;


	return (*this);
}


//Getters and setters hellish nightmare
// PD: Now I have to enter this nightmare as well. The quest for an efficient code has started
// PD2: Ferran stop with this comments, we don't understand anything you say

int Hero::GetHeroLevel() const
{
	return stats.heroLevel;
}


void Hero::SetHeroLevel(int lvl)
{
	stats.heroLevel = lvl;
}


int Hero::GetExpToLevelUp() const
{
	return stats.xpToLvlUp;
}


void Hero::SetExpToLevelUp(int exp)
{
	stats.xpToLvlUp = exp;
}


int Hero::GetHeroXP() const
{
	return heroXP;
}


void Hero::SetHeroXP(int xp)
{
	heroXP = xp;
}


int Hero::GetRecoveryHitPointsRate() const
{
	return stats.recoveryHPRate;
}


void Hero::SetRecoveryHitPointsRate(int hpRate)
{
	stats.recoveryHPRate = hpRate;
}


int Hero::GetEnergyPoints() const
{
	return stats.currEnergy;
}


void Hero::SetEnergyPoints(int engPoints)
{
	stats.currEnergy = engPoints;
}


void Hero::AddEnergyPoints(int engPoints)
{
	stats.currEnergy += engPoints;
}


int Hero::GetMaxEnergyPoints() const
{
	return stats.maxEnergy;
}


void Hero::SetMaxEnergyPoints(int maxEngPoints)
{
	stats.maxEnergy = maxEngPoints;
}


int Hero::GetRecoveryEnergyRate() const
{
	return stats.recoveryEnergyRate;
}


void Hero::SetRecoveryEnergyRate(int recEnergyRate)
{
	stats.recoveryEnergyRate = recEnergyRate;
}


float Hero::GetRecoveringHealth() const
{
	return recoveringHealth;
}


void Hero::SetRecoverHealth(float recHealth)
{
	recoveringHealth = recHealth;
}


float Hero::GetRecoveringEnergy() const
{
	return recoveringEnergy;
}


void Hero::SetRecoveringEnergy(float recoverEnergy)
{
	recoveringEnergy = recoverEnergy;
}


float Hero::GetFeelingSecure() const
{
	return feelingSecure;
}


void Hero::SetFeelingSecure(float feelSecure)
{
	feelingSecure = feelSecure;
}


float Hero::GetAttackDamage() const
{
	return stats.damage;
}


void Hero::SetAttackDamage(float atkDamage)
{
	stats.damage = atkDamage;
}


int Hero::GetAttackRange() const
{
	return stats.attackRange;
}


void Hero::SetAttackRange(int atkRange)
{
	stats.attackRange = atkRange;
}


int Hero::GetSkill1Cost() const
{
	return skill1.energyCost;
}


void Hero::SetSkill1Cost(int skillCost)
{
	skill1.energyCost = skillCost;
}

Skill Hero::GetSkill1() const
{
	return skill1;
}

void Hero::ReplaceSkill1(Skill newSkill)
{
	skill1 = newSkill;
}

void Hero::ReplaceHeroStats(HeroStats newStats)
{
	stats = newStats;
}


float Hero::GetAttackSpeed() const
{
	return stats.atkSpeed;
}


void Hero::SetAttackSpeed(float atkSpeed)
{
	stats.atkSpeed = atkSpeed;
}


float Hero::GetSkill1RecoverTime() const
{
	return skill1.coolDown;
}


void Hero::SetSkill1RecoverTime(float skillRecoverTime)
{
	skill1RecoverTime = skill1.coolDown;;
}


float Hero::GetSkill2RecoverTime() const
{
	return skill2RecoverTime;
}


void Hero::SetSkill2RecoverTime(float skillRecoverTime)
{
	skill2RecoverTime = skillRecoverTime;
}


float Hero::GetSkill3RecoverTime() const
{
	return skill3RecoverTime;
}


void Hero::SetSkill3RecoverTime(float skillRecoverTime)
{
	skill3RecoverTime = skillRecoverTime;
}

int Hero::GetMaxHP() const
{
	return stats.maxHP;
}

void Hero::SetMaxHP(int newMaxHp)
{
	stats.maxHP = newMaxHp;
}

int Hero::GetCurrentHP() const
{
	return stats.currHP;
}

void Hero::SetCurrentHP(int newcurrHp)
{
	stats.currHP = newcurrHp;
}


float Hero::GetSkill1TimePassed() const
{
	return skill1TimePassed;
}


float Hero::GetSkill2TimePassed() const
{
	return skill2TimePassed;
}


float Hero::GetSkill3TimePassed() const
{
	return skill3TimePassed;
}


int Hero::GetVisionDistance() const
{
	return stats.visionDistance;
}


void Hero::SetVisionDistance(int visDistance)
{
	stats.visionDistance = visDistance;
}


float Hero::GetVisionInPx() const
{
	return visionInPx;
}


void Hero::SetVisionInPx(float visPx)
{
	visionInPx = visPx;
}


DeadHero::DeadHero(int level, ENTITY_TYPE type, Skill skill) : level(level), heroType(type)
{
	skillLevel = skill.lvl;
	skillId = skill.id;
}


DeadHero::~DeadHero()
{}


ENTITY_TYPE DeadHero::GetType() const
{
	return heroType;
}


int DeadHero::GetLevel() const
{
	return level;
}

void DeadHero::GetSkillInfo(SKILL_ID& id, int& newskillLevel) const
{
	id = skillId;
	newskillLevel = skillLevel;
}


int Hero::GetHeroSkillPoints()
{
	return heroSkillPoints;
}


void Hero::SetHeroSkillPoints(int n)
{
	heroSkillPoints = n;
}


void Hero::AddHeroSkillPoints(int n)
{
	heroSkillPoints += n;
}
