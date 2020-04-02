#include "Enemy.h"
#include "App.h"
#include "AI.h"
#include "EntityManager.h"
#include "Textures.h"
#include "Render.h"

Enemy::Enemy(fMPoint position, ENTITY_TYPE type, Collider* collider, Animation& animation, int hitPoints, int recoveryHitPointsRate,
	int vision, int attackDamage, int attackSpeed, int attackRange, int movementSpeed, int xpOnDeath) :

	DynamicEntity(position, type, collider, 10, 20),
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

	attackCharged(true),
	haveOrders(false),

	state(ENEMY_STATES::IDLE)
{}


Enemy::Enemy(fMPoint position, Enemy* copy) :

	DynamicEntity(position, copy->type, copy->collider, copy->moveRange1, copy->moveRange2),
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

	attackCharged(true),
	haveOrders(false),

	state(ENEMY_STATES::IDLE)
{}


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
	externalInput(inputs, dt);
	internalInput(inputs, dt);
	state = processFsm(inputs);

	switch (state)
	{
	case ENEMY_STATES::IDLE:

		break;

	case ENEMY_STATES::MOVE:
		Move();
		break;

	case ENEMY_STATES::ATTACK:
		if (attackCooldown == 0)
		{
			Attack();
			attackCooldown += dt;

		}
		inputs.push_back(ENEMY_INPUTS::IN_CHARGING_ATTACK);
		break;

	case ENEMY_STATES::CHARGING_ATTACK:
		break;

	case ENEMY_STATES::DEAD:
		Die();
		break;
	}

	collider->SetPos((int)position.x, (int)position.y);

	return true;
}


bool Enemy::PostUpdate(float dt)
{
	Draw(dt);
	return true;
}


bool Enemy::MoveTo(int x, int y)
{
	//do pathfinding, if it works return true
	if (GeneratePath(x, y))
	{
		inputs.push_back(ENEMY_INPUTS::IN_MOVE);
		return true;
	}

	return false;
}


Enemy* Enemy::Clone()
{
	return new Enemy(this->position, this);
}

Enemy* Enemy::Clone(fMPoint positionToBe)
{
	return  new Enemy(positionToBe, this);
}

Enemy* Enemy::Clone(int x, int y)
{
	fMPoint newPos(x, y);
	return  new Enemy(newPos, this);
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
	app->render->Blit(texture, position.x, position.y, &animation.GetCurrentFrameBox(dt));
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
	SDL_Rect rect;

	rect.x = position.x - vision;
	rect.y = position.y - vision;
	rect.w = vision * 2;
	rect.h = vision * 2;

	shortTermObjective = app->entityManager->CheckEnemyObjective(&rect);

	if (shortTermObjective != nullptr)
	{
		return true;
	}

	return false;
}


bool Enemy::CheckAttackRange()
{
	if (shortTermObjective == nullptr)
	{
		return false;
	}

	int distance = abs(abs(shortTermObjective->GetPosition().x) + abs(shortTermObjective->GetPosition().y) - (abs(position.x) + abs(position.y)));

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


void Enemy::internalInput(std::vector<ENEMY_INPUTS>& inputs, float dt)
{
	if (attackCooldown > 0)
	{
		if (attackCooldown >= attackSpeed)
		{
			attackCharged = true;
			attackCooldown = 0;
		}

		attackCooldown += dt;
	}
}


bool Enemy::externalInput(std::vector<ENEMY_INPUTS>& inputs, float dt)
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


ENEMY_STATES Enemy::processFsm(std::vector<ENEMY_INPUTS>& inputs)
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