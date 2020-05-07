#include "Turret.h"
#include "EntityManager.h"
#include "Render.h"
#include "Map.h"
#include "FoWManager.h"
#include "Base.h"
#include "EventManager.h"


Turret::Turret(int turretLvl, int attackDmg, int attackSpeed, int range, int vision, fMPoint position, Collider* collider, Animation& idleRight, Animation& idleRightUp, Animation& idleRightDown, Animation& idleLeft,
	Animation& idleLeftUp, Animation& idleLeftDown, Animation& shootingRight, Animation& shootingRightUp, Animation& shootingRightDown, Animation& shootingLeft, Animation& shootingLeftUp,
	Animation& shootingLeftDown, int maxHitPoints, int currentHitPoints, int recoveryHitPointsRate, int xpOnDeath, int buildingCost, int transparency) :

	Building(position, maxHitPoints, currentHitPoints, recoveryHitPointsRate, xpOnDeath, buildingCost, transparency, collider, ENTITY_TYPE::BLDG_TURRET),

	shootingLeft(shootingLeft),
	shootingLeftUp(shootingLeftUp),
	shootingLeftDown(shootingLeftDown),
	shootingRightUp(shootingRightUp),
	shootingRightDown(shootingRightDown),
	shootingRight(shootingRight),
	idleRight(idleRight),
	idleRightDown(idleRightDown),
	idleRightUp(idleRightUp),
	idleLeft(idleLeft),
	idleLeftUp(idleLeftUp),
	idleLeftDown(idleLeftDown),

	turretLvl(turretLvl),
	attackDmg(attackDmg),
	attackSpeed(attackSpeed),
	range(range),
	vision(vision),

	attackCD(0),

	shortTermObjective(nullptr),

	state(TURRET_STATES::IDLE)
{
	currentAnimation = &this->idleRightDown;
}


Turret::Turret(fMPoint position, Turret* copy, ENTITY_ALIGNEMENT alignement) :

	Building(position, copy, alignement),


	shootingLeft(copy->shootingLeft),
	shootingLeftUp(copy->shootingLeftUp),
	shootingLeftDown(copy->shootingLeftDown),
	shootingRightUp(copy->shootingRightUp),
	shootingRightDown(copy->shootingRightDown),
	shootingRight(copy->shootingRight),
	idleRight(copy->idleRight),
	idleRightDown(copy->idleRightDown),
	idleRightUp(copy->idleRightUp),
	idleLeft(copy->idleLeft),
	idleLeftUp(copy->idleLeftUp),
	idleLeftDown(copy->idleLeftDown),

	turretLvl(copy->turretLvl),
	attackDmg(copy->attackDmg),
	attackSpeed(copy->attackSpeed),
	range(copy->range),
	vision(copy->vision),
	attackCD(0),


	shortTermObjective(nullptr),

	state(TURRET_STATES::IDLE)
{
	currentAnimation = &idleRightDown;

	this->visionEntity = app->fowManager->CreateFoWEntity(this->position, true, vision);


}


Turret::~Turret()
{
	shortTermObjective = nullptr;
	currentAnimation = nullptr;

	inputs.clear();

}


bool Turret::PreUpdate(float dt)
{
	transparent = false;
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
	if (app->debugMode)
	{
		//Position --------------------------------------
		app->render->DrawQuad({ (int)position.x, (int)position.y, 2,2 }, 255, 0, 0);

		fMPoint nextPoint = { 0,0 };
		iMPoint origin = app->map->WorldToMap(round(position.x), round(position.y));
		origin = app->map->MapToWorld(origin.x, origin.y);

		app->render->DrawQuad({ (int)origin.x, (int)origin.y, 10,10 }, 255, 255, 255, 125);

	}
	return true;
}


void Turret::CheckObjective(Entity* entity)
{
	if (shortTermObjective == entity)
	{
		shortTermObjective = nullptr;

		SearchObjective();
		inputs.push_back(TURRET_INPUTS::IN_IDLE);
	}
}


bool Turret::SearchObjective()
{
	bool ret = false;
	SDL_Rect rect;

	rect.x = position.x - range;
	rect.y = position.y - center.y - range;
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
	if (transparent)
	{
		app->render->Blit(texture, position.x, position.y , &currentAnimation->GetCurrentFrameBox(dt), false, true, transparencyValue, 255, 255, 255, 1.0f, -offset.x, -offset.y);
	}
	else
		app->render->Blit(texture, position.x, position.y, &currentAnimation->GetCurrentFrameBox(dt), false, true, 0,255,255,255, 1.0f, -offset.x, -offset.y);
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

void Turret::DrawSelected()
{
	if (selectedByPlayer == true)
		app->render->Blit(app->entityManager->selectedTexture, this->collider->rect.x + this->collider->rect.w * 0.5f, this->collider->rect.y);
}

int Turret::RecieveDamage(int damage)
{
	if (hitPointsCurrent > 0)
	{
		hitPointsCurrent -= damage;

		int randomCounter = rand() % 10;

		if (randomCounter == 0)
			app->audio->PlayFx(app->entityManager->buildingGetsHit, 0, 3, this->GetMyLoudness(), this->GetMyDirection(), true);
		else if (randomCounter == 9)
			app->audio->PlayFx(app->entityManager->buildingGetsHit2, 0, 3, this->GetMyLoudness(), this->GetMyDirection(), true);

		if (hitPointsCurrent <= 0)
		{
			Die();
		}
	}

	return 0;
}


bool Turret::CheckAttackRange()
{
	if (shortTermObjective == nullptr)
	{
		inputs.push_back(TURRET_INPUTS::IN_IDLE);
		return false;
	}


	if (shortTermObjective->GetAlignment() == align)
	{
		shortTermObjective = nullptr;
		inputs.push_back(TURRET_INPUTS::IN_IDLE);
		return false;
	}

	SDL_Rect rect;
	rect.x = position.x - range;
	rect.y = position.y - center.y - range;
	rect.w = range * 2;
	rect.h = range * 2;


	if (shortTermObjective->GetCollider()->CheckCollision(rect))
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
	if (shortTermObjective)
		shortTermObjective->RecieveDamage(attackDmg);
}


void Turret::Die()
{
	app->eventManager->GenerateEvent(EVENT_ENUM::ENTITY_DEAD, EVENT_ENUM::NULL_EVENT);
	toDelete = true;

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

	if (myBase != nullptr)
	{
		myBase->RemoveTurret(this);
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

			case TURRET_INPUTS::IN_OBJECTIVE_DONE:		state = TURRET_STATES::IDLE;			break;

			case TURRET_INPUTS::IN_OUT_OF_RANGE:		state = TURRET_STATES::IDLE;			break;

			case TURRET_INPUTS::IN_IDLE:				state = TURRET_STATES::IDLE;			break;

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

			case TURRET_INPUTS::IN_IDLE:			state = TURRET_STATES::IDLE;				break;

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
			if (shortTermObjective != nullptr)
				dir = DetermineDirection(shortTermObjective->position - position);

			attackCD += 0.01f;
		}
		else
		{
			inputs.push_back(TURRET_INPUTS::IN_CHARGING_ATTACK);
		}

		break;

	case TURRET_STATES::CHARGING_ATTACK:

		app->audio->PlayFx(app->entityManager->turretShooting, 0, -1, this->GetMyLoudness(), this->GetMyDirection());

		break;

	case TURRET_STATES::DEAD:
		Die();
		break;
	}

	SetAnimation(state);
}

FACE_DIR Turret::DetermineDirection(fMPoint faceDir)
{
	FACE_DIR newDir = dir;

	if (faceDir.x > 0)
	{
		if (faceDir.y < -0.1f)
		{
			newDir = FACE_DIR::NORTH_EAST;

		}
		else if (faceDir.y > 0.1f)
		{
			newDir = FACE_DIR::SOUTH_EAST;
		}
		else
		{
			newDir = FACE_DIR::EAST;
		}
	}
	else if (faceDir.x < 0)
	{
		if (faceDir.y < -0.1f)
		{
			newDir = FACE_DIR::NORTH_WEST;
		}
		else if (faceDir.y > 0.1f)
		{
			newDir = FACE_DIR::SOUTH_WEST;

		}
		else
		{
			newDir = FACE_DIR::WEST;
		}
	}


	return newDir;
}

void Turret::SetAnimation(TURRET_STATES state)
{
	switch (state)
	{
	case TURRET_STATES::IDLE:
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
	}
	break;
	case TURRET_STATES::ATTACK:
	{
		switch (dir)
		{
		case FACE_DIR::NORTH_EAST:
			currentAnimation = &shootingRightUp;
			break;
		case FACE_DIR::NORTH_WEST:
			currentAnimation = &shootingLeftUp;
			break;
		case FACE_DIR::EAST:
			currentAnimation = &shootingRight;
			break;
		case FACE_DIR::SOUTH_EAST:
			currentAnimation = &shootingRightDown;
			break;
		case FACE_DIR::SOUTH_WEST:
			currentAnimation = &shootingLeftDown;
			break;
		case FACE_DIR::WEST:
			currentAnimation = &shootingLeft;
			break;
		}
		break;
	}
	}
}


void Turret::SetLevel(int lvl)
{
	for (int i = 1; i < lvl; i++)
	{
		//LevelUp() TODO
	}

	turretLvl = lvl;
}