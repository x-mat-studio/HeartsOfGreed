#include "Enemy.h"
#include "App.h"
#include "AI.h"
#include "EntityManager.h"
#include "FoWManager.h"
#include "Textures.h"
#include "Render.h"

Enemy::Enemy(fMPoint position, ENTITY_TYPE type, Collider* collider, Animation& animation, int hitPoints, int recoveryHitPointsRate,
	int vision, int attackDamage, int attackSpeed, int attackRange, int movementSpeed, int xpOnDeath) :

	DynamicEntity(position, type, ENTITY_ALIGNEMENT::NEUTRAL, collider, 5, 10),
	animation(animation),

	hitPoints(hitPoints),
	recoveryHitPointsRate(recoveryHitPointsRate),
	vision(vision),
	attackDamage(attackDamage),
	attackSpeed(attackSpeed),
	attackRange(attackRange),
	attackCooldown(0),

	movementSpeed(movementSpeed),
	xpOnDeath(xpOnDeath),
	longTermObjective{ NULL, NULL },
	shortTermObjective(nullptr),

	haveOrders(false),

	state(ENEMY_STATES::IDLE)
{}


Enemy::Enemy(fMPoint position, Enemy* copy, ENTITY_ALIGNEMENT align) :

	DynamicEntity(position, copy->type, align, copy->collider, copy->moveRange1, copy->moveRange2),
	animation(copy->animation),

	hitPoints(copy->hitPoints),
	recoveryHitPointsRate(copy->recoveryHitPointsRate),
	vision(copy->vision),
	attackDamage(copy->attackDamage),
	attackSpeed(copy->attackSpeed),
	attackRange(copy->attackRange),
	attackCooldown(0),

	movementSpeed(copy->movementSpeed),
	xpOnDeath(copy->xpOnDeath),
	longTermObjective{ NULL, NULL },
	shortTermObjective(nullptr),

	haveOrders(false),

	state(ENEMY_STATES::IDLE)
{
	//FoW Related
	visionEntity = app->fowManager->CreateFoWEntity(position, false);
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
				if (framePathfindingCount == framesPerPathfinding)
				{
					fMPoint pos = shortTermObjective->GetPosition();
					MoveTo(pos.x, pos.y);
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


bool Enemy::PostUpdate(float dt)
{
	return true;
}


bool Enemy::MoveTo(int x, int y)
{
	//do pathfinding, if it works return true
	framePathfindingCount = 0;

	if (GeneratePath(x, y))
	{
		inputs.push_back(ENEMY_INPUTS::IN_MOVE);
		return true;
	}

	return false;
}


Enemy* Enemy::Clone()
{
	return new Enemy(this->position, this, ENTITY_ALIGNEMENT::NEUTRAL);
}

Enemy* Enemy::Clone(fMPoint positionToBe)
{
	return  new Enemy(positionToBe, this, ENTITY_ALIGNEMENT::NEUTRAL);
}

Enemy* Enemy::Clone(int x, int y)
{
	fMPoint newPos(x, y);
	return  new Enemy(newPos, this, ENTITY_ALIGNEMENT::NEUTRAL);
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
	app->render->Blit(texture, position.x - offset.x, position.y - offset.y, &animation.GetCurrentFrameBox(dt));
}


void Enemy::Attack()
{
	//i have to think about this one

}


void Enemy::Die()
{
	app->entityManager->AddEvent(EVENT_ENUM::ENTITY_DEAD);
	toDelete = true;
}


void Enemy::CheckObjecive(Entity* entity)
{
	if (shortTermObjective == entity)
	{
		shortTermObjective == nullptr;
	}
}



void Enemy::RecoverHealth()
{

}


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

	if (objective != nullptr && shortTermObjective != objective)
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

	int distanceX = abs(position.x - point.x);
	int distanceY = abs(position.y - point.y);


	int distance = distanceX + distanceY;

	if (distance < attackRange)
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