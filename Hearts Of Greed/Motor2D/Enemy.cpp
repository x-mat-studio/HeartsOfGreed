#include "Enemy.h"
#include "App.h"
#include "AI.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "FoWManager.h"
#include "Audio.h"
#include "Textures.h"
#include "Render.h"
#include "Input.h"

Enemy::Enemy(fMPoint position, ENTITY_TYPE type, Collider* collider, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
	Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightUp, Animation& idleRightDown, Animation& idleLeft, Animation& idleLeftUp, Animation& idleLeftDown,
	Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp, Animation& punchRightDown, Animation& punchRight, int maxHitPoints, int currentHitPoints,
	int recoveryHitPointsRate, int vision, int attackDamage, int attackSpeed, int attackRange, int movementSpeed, int xpOnDeath) :

	DynamicEntity(position, movementSpeed, type, ENTITY_ALIGNEMENT::NEUTRAL, collider, maxHitPoints, currentHitPoints, 10, 20),
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

	recoveryHitPointsRate(recoveryHitPointsRate),
	vision(vision),
	attackDamage(attackDamage),
	attackSpeed(attackSpeed),
	attackRange(attackRange),
	attackCooldown(0),

	framePathfindingCount(0),
	framesPerPathfinding(FRAMES_PER_PATHFINDING),

	xpOnDeath(xpOnDeath),
	longTermObjective{ NULL, NULL },
	shortTermObjective(nullptr),
	damageTakenTimer(0.f),
	haveOrders(false),

	state(ENEMY_STATES::IDLE)
{}


Enemy::Enemy(fMPoint position, Enemy* copy, ENTITY_ALIGNEMENT align) :

	DynamicEntity(position, copy->unitSpeed, copy->type, align, copy->collider, copy->hitPointsMax, copy->hitPointsCurrent, copy->moveRange1, copy->moveRange2),
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

	recoveryHitPointsRate(copy->recoveryHitPointsRate),
	vision(copy->vision),
	attackDamage(copy->attackDamage),
	attackSpeed(copy->attackSpeed),
	attackRange(copy->attackRange),
	attackCooldown(0),

	framePathfindingCount(0),
	framesPerPathfinding(FRAMES_PER_PATHFINDING),

	xpOnDeath(copy->xpOnDeath),
	longTermObjective{ NULL, NULL },
	shortTermObjective(nullptr),
	damageTakenTimer(0.f),

	haveOrders(false),

	state(ENEMY_STATES::IDLE)
{
	//FoW Related
	visionEntity = app->fowManager->CreateFoWEntity(position, false); //TODO this is going to be the enemy vision distance
	currentAnimation = &idleRightDown;

	int randomCounter = rand() % 50;
	framesPerPathfinding += randomCounter;
}


Enemy::~Enemy()
{
	shortTermObjective = nullptr;

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
}


bool Enemy::PreUpdate(float dt)
{


	return true;
}


bool Enemy::Update(float dt)
{
	//check inputs to traverse state matrix
	ExternalInput(inputs, dt);
	InternalInput(inputs, dt);
	state = ProcessFsm(inputs);

	StateMachine(dt);
	GroupMovement(dt);

	Roar();
	DrawOnSelect();
	CollisionPosUpdate();


	return true;
}


void Enemy::StateMachine(float dt)
{
	switch (state)
	{
	case ENEMY_STATES::IDLE:

		break;

	case ENEMY_STATES::MOVE:

		if (Move(dt) == true)
		{
			if (shortTermObjective == nullptr)
			{
				inputs.push_back(ENEMY_INPUTS::IN_IDLE);
			}
			else
			{
				if (framePathfindingCount == framesPerPathfinding && shortTermObjective != nullptr)
				{
					fMPoint pos = shortTermObjective->GetPosition();
					fMPoint offSet = shortTermObjective->GetCenter();

					MoveTo(pos.x + offSet.x, pos.y + offSet.y);
				}
			}
		}
		else
			inputs.push_back(ENEMY_INPUTS::IN_IDLE);

		visionEntity->SetNewPosition(position);
		break;

	case ENEMY_STATES::ATTACK:

		if (attackCooldown == 0)
		{
			Attack();
			if (shortTermObjective != nullptr)
				dir = DetermineDirection(shortTermObjective->position - position);

			attackCooldown += 0.01f;
		}
		else
		{
			inputs.push_back(ENEMY_INPUTS::IN_CHARGING_ATTACK);
		}
		break;

	case ENEMY_STATES::CHARGING_ATTACK:
		break;

	case ENEMY_STATES::DEAD:
		Die();
		break;
	}

	CollisionPosUpdate();

	SetAnimation(state);
}

void Enemy::Roar()
{
	//DEBUGSOUND
	int randomCounter = rand() % 1000;

	if (randomCounter == 997) {

		app->audio->PlayFx(app->entityManager->wanamingoRoar, 0, 1, this->GetMyLoudness(), this->GetMyDirection());

	}
	if (randomCounter == 998) {

		app->audio->PlayFx(app->entityManager->wanamingoRoar2, 0, 2, this->GetMyLoudness(), this->GetMyDirection());

	}
}

void Enemy::DrawOnSelect()
{
	if(selected_by_player)
	app->render->Blit(app->entityManager->target, this->collider->rect.x + this->collider->rect.w / 2, this->collider->rect.y);
}


bool Enemy::PostUpdate(float dt)
{
	return true;
}


bool Enemy::MoveTo(float x, float y)
{

	framePathfindingCount = 0;

	if (GeneratePath(x, y, 1))
	{
		inputs.push_back(ENEMY_INPUTS::IN_MOVE);
		return true;
	}

	return false;
}


void Enemy::OnCollision(Collider* collider)
{
	if (collider->type == COLLIDER_RECLUIT_IA)
	{
		longTermObjective = *app->ai->GetObjective();
		haveOrders = true;
	}
}


void Enemy::Draw(float dt)
{
	Frame currFrame;

	if (state == ENEMY_STATES::CHARGING_ATTACK)
		currFrame = currentAnimation->GetCurrentFrame();
	else
		currFrame = currentAnimation->GetCurrentFrame(dt);

	if (damageTakenTimer > 0.f)
		app->render->Blit(texture, position.x - currFrame.pivotPositionX, position.y - currFrame.pivotPositionY, &currFrame.frame, false, true, 0, 255, 0, 0/*, -currFrame.pivotPositionX, -currFrame.pivotPositionY*/);

	else
		app->render->Blit(texture, position.x - currFrame.pivotPositionX, position.y - currFrame.pivotPositionY, &currFrame.frame, false, true, 0, 255, 255, 255/*, -currFrame.pivotPositionX, -currFrame.pivotPositionY*/);

	DebugDraw(currFrame.pivotPositionX, currFrame.pivotPositionY);
}


void Enemy::Attack()
{
	if (shortTermObjective)
		shortTermObjective->RecieveDamage(attackDamage);
}


void Enemy::Die()
{
	toDelete = true;
	app->eventManager->GenerateEvent(EVENT_ENUM::ENTITY_DEAD, EVENT_ENUM::NULL_EVENT);
	collider->thisEntity = nullptr;

	int randomCounter = rand() % 2;

	if (randomCounter == 1)
	{
		app->audio->PlayFx(app->entityManager->wanamingoDies, 0, 1, this->GetMyLoudness(), this->GetMyDirection());
	}
	else
	{
		app->audio->PlayFx(app->entityManager->wanamingoDies2, 0, 2, this->GetMyLoudness(), this->GetMyDirection());
	}

	if (minimapIcon != nullptr)
	{
		minimapIcon->toDelete = true;
		minimapIcon->minimapPos = nullptr;
	}
}


void Enemy::CheckObjecive(Entity* entity)
{
	if (shortTermObjective == entity)
	{
		path.clear();
		DestroyPath();
		shortTermObjective = nullptr;
		SearchObjective();

		inputs.push_back(ENEMY_INPUTS::IN_IDLE);
	}
}

void Enemy::SearchForNewObjective()
{
	shortTermObjective = app->entityManager->SearchUnitsInRange(vision, this);
}

void Enemy::RecoverHealth()
{}


bool Enemy::SearchObjective()
{
	bool ret = false;

	SDL_Rect rect;

	rect.x = position.x - vision;
	rect.y = position.y - vision;
	rect.w = vision * 2;
	rect.h = vision * 2;

	Entity* objective;
	objective = app->entityManager->SearchEntityRect(&rect, align);

	if (objective != nullptr)
	{
		ret = true;
	}

	shortTermObjective = objective;

	return ret;
}


bool Enemy::CheckAttackRange()
{
	if (shortTermObjective == nullptr)
	{
		return false;
	}

	if (shortTermObjective->GetAlignment() == align)
	{
		shortTermObjective = nullptr;
		return false;
	}

	SDL_Rect rect;
	rect.x = position.x - attackRange;
	rect.y = position.y - attackRange;
	rect.w = attackRange * 2;
	rect.h = attackRange * 2;


	if (shortTermObjective->GetCollider()->CheckCollision(rect))
	{
		return true;
	}
	else
	{
		inputs.push_back(ENEMY_INPUTS::IN_OUT_OF_RANGE);
		return false;
	}
}


void Enemy::InternalInput(std::vector<ENEMY_INPUTS>& inputs, float dt)
{
	if (attackCooldown > 0)
	{
		attackCooldown += dt;

		currentAnimation->GetCurrentFrame(attackSpeed * dt);

		if (&currentAnimation->GetCurrentFrame() == &currentAnimation->frames[currentAnimation->lastFrame - 1])
		{
			inputs.push_back(ENEMY_INPUTS::IN_ATTACK_CHARGED);
			attackCooldown = 0;
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


bool Enemy::ExternalInput(std::vector<ENEMY_INPUTS>& inputs, float dt)
{
	if (CheckAttackRange())
	{
		inputs.push_back(ENEMY_INPUTS::IN_ATTACK);
	}

	else
	{
		if (framePathfindingCount == framesPerPathfinding)
		{
			if (SearchObjective() == true)
			{
				MoveTo(shortTermObjective->GetPosition().x, shortTermObjective->GetPosition().y);
			}

			else if (haveOrders)
			{
				MoveTo(longTermObjective.x, longTermObjective.y);
			}
		}
	}

	return true;
}


ENEMY_STATES Enemy::ProcessFsm(std::vector<ENEMY_INPUTS>& inputs)
{
	ENEMY_INPUTS lastInput;

	while (inputs.empty() == false)
	{
		lastInput = inputs.back();
		inputs.pop_back();

		switch (state)
		{

		case ENEMY_STATES::IDLE:
		{
			switch (lastInput)
			{
			case ENEMY_INPUTS::IN_MOVE:   state = ENEMY_STATES::MOVE;		break;

			case ENEMY_INPUTS::IN_ATTACK: state = ENEMY_STATES::ATTACK;		break;

			case ENEMY_INPUTS::IN_DEAD:   state = ENEMY_STATES::DEAD;		break;
			}
		}	break;


		case ENEMY_STATES::MOVE:
		{
			switch (lastInput)
			{
			case ENEMY_INPUTS::IN_IDLE:   state = ENEMY_STATES::IDLE;		break;

			case ENEMY_INPUTS::IN_ATTACK: state = ENEMY_STATES::ATTACK;		break;

			case ENEMY_INPUTS::IN_DEAD:   state = ENEMY_STATES::DEAD;		break;
			}
		}	break;


		case ENEMY_STATES::ATTACK:
		{
			switch (lastInput)
			{
			case ENEMY_INPUTS::IN_CHARGING_ATTACK: state = ENEMY_STATES::CHARGING_ATTACK;	break;

			case ENEMY_INPUTS::IN_DEAD:			   state = ENEMY_STATES::DEAD;				break;
			}
		}	break;


		case ENEMY_STATES::CHARGING_ATTACK:
		{
			switch (lastInput)
			{
			case ENEMY_INPUTS::IN_ATTACK_CHARGED: state = ENEMY_STATES::ATTACK;				break;

			case ENEMY_INPUTS::IN_OBJECTIVE_DONE:  state = ENEMY_STATES::IDLE;				break;

			case ENEMY_INPUTS::IN_OUT_OF_RANGE:    state = ENEMY_STATES::MOVE;				break;

			case ENEMY_INPUTS::IN_MOVE:			   state = ENEMY_STATES::MOVE;				break;

			case ENEMY_INPUTS::IN_DEAD:			   state = ENEMY_STATES::DEAD;				break;
			}
		}	break;


		case ENEMY_STATES::DEAD:
		{
		}	break;

		}

	}

	return state;

}

int Enemy::RecieveDamage(int damage)
{
	int ret = -1;

	if (hitPointsCurrent > 0)
	{
		hitPointsCurrent -= damage;
		damageTakenTimer = 0.3f;

		if (hitPointsCurrent <= 0)
		{
			Die();
			ret = xpOnDeath;
		}
		else
			app->audio->PlayFx(app->entityManager->wanamingoGetsHit, 0, 3, this->GetMyLoudness(), this->GetMyDirection(), true);
	}

	return ret;
}


void Enemy::SetAnimation(ENEMY_STATES state)
{
	switch (state)
	{
	case ENEMY_STATES::MOVE:
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
	case ENEMY_STATES::IDLE:
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

	case ENEMY_STATES::CHARGING_ATTACK:
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
		break;
	}

	}
}


int Enemy::GetHP()
{
	return hitPointsCurrent;
}

int Enemy::GetAD()
{
	return attackDamage;
}

int Enemy::GetAS()
{
	return attackSpeed;
}

int Enemy::GetVision()
{
	return vision;
}

int Enemy::GetRecov()
{
	return recoveryHitPointsRate;
}