#include "Enemy.h"
#include "App.h"
#include "AI.h"
#include "EntityManager.h"
#include "FoWManager.h"
#include "Audio.h"
#include "Textures.h"
#include "Render.h"

Enemy::Enemy(fMPoint position, ENTITY_TYPE type, Collider* collider, Animation& animation, int hitPoints, int recoveryHitPointsRate,
	int vision, int attackDamage, int attackSpeed, int attackRange, int movementSpeed, int xpOnDeath) :

	DynamicEntity(position, movementSpeed, type, ENTITY_ALIGNEMENT::NEUTRAL, collider, 10, 20),
	animation(animation),

	hitPoints(hitPoints),
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

	DynamicEntity(position, copy->unitSpeed, copy->type, align, copy->collider, copy->moveRange1, copy->moveRange2),
	animation(copy->animation),

	hitPoints(copy->hitPoints),
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
	visionEntity = app->fowManager->CreateFoWEntity(position, false, 3);//TODO this is going to be the enemy vision distance
}


Enemy::~Enemy()
{
	shortTermObjective = nullptr;

	inputs.clear();

	animation = Animation();
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
		visionEntity->SetNewPosition(position);
		break;

	case ENEMY_STATES::ATTACK:

		if (attackCooldown == 0)
		{
			Attack();
			attackCooldown += 0.1;
		}

		inputs.push_back(ENEMY_INPUTS::IN_CHARGING_ATTACK);
		break;

	case ENEMY_STATES::CHARGING_ATTACK:
		break;

	case ENEMY_STATES::DEAD:
		Die();
		break;
	}

	CollisionPosUpdate();
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
	if (damageTakenTimer > 0.f)
		app->render->Blit(texture, position.x - offset.x, position.y - offset.y, &animation.GetCurrentFrameBox(dt), 0, 255,0,0);
	else
		app->render->Blit(texture, position.x - offset.x, position.y - offset.y, &animation.GetCurrentFrameBox(dt));

	DebugDraw();
}


void Enemy::Attack()
{
	if (shortTermObjective)
		shortTermObjective->RecieveDamage(attackDamage);
}


void Enemy::Die()
{
	app->entityManager->AddEvent(EVENT_ENUM::ENTITY_DEAD);
	toDelete = true;
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
}


void Enemy::CheckObjecive(Entity* entity)
{
	if (shortTermObjective == entity)
	{
		path.clear();
		shortTermObjective = nullptr;
		SearchForNewObjective();

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


	fMPoint point = shortTermObjective->GetPosition();

	if (point.DistanceManhattan(position) < attackRange)
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

		if (attackCooldown >= attackSpeed)
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
		if (SearchObjective() == true)
		{
			MoveTo(shortTermObjective->GetPosition().x, shortTermObjective->GetPosition().y);
		}

		else if (haveOrders)
		{
			MoveTo(longTermObjective.x, longTermObjective.y);
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

	if (hitPoints > 0)
	{
		hitPoints -= damage;
		damageTakenTimer = 0.3f;

		if (hitPoints <= 0)
		{
			Die();
			ret = xpOnDeath;
		}
		else
			app->audio->PlayFx(app->entityManager->wanamingoGetsHit, 0, 3, this->GetMyLoudness(), this->GetMyDirection());
	}

	return ret;
}