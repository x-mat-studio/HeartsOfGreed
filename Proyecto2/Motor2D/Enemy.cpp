#include "Enemy.h"
#include "App.h"
#include "AI.h"
#include "EntityManager.h"
#include "Textures.h"
#include "Render.h"

Enemy::Enemy(SDL_Point position, ENTITY_TYPE type, SDL_Texture* texture, Collider* collider, Animation& animation,
	int hitPoints, int recoveryHitPointsRate, int attackDamage, int attackSpeed, int attackRange, int movementSpeed,
	int xpOnDeath) :

	Entity(position, type, collider),
	animation(animation),
	hitPoints(hitPoints),
	recoveryHitPointsRate(recoveryHitPointsRate),
	attackDamage(attackDamage),
	attackSpeed(attackSpeed),
	attackRange(attackRange),
	movementSpeed(movementSpeed),
	xpOnDeath(xpOnDeath),
	longTermObjective{ NULL, NULL },
	shortTermObjective(nullptr),
	attackCooldown(attackCooldown)
{}


Enemy::Enemy(SDL_Point position, Enemy* copy) :

	Entity(position, copy->type, copy->GetCollider()),
	animation(copy->animation),
	hitPoints(copy->hitPoints),
	recoveryHitPointsRate(copy->recoveryHitPointsRate),
	attackDamage(copy->attackDamage),
	attackSpeed(copy->attackSpeed),
	attackRange(copy->attackRange),
	movementSpeed(copy->movementSpeed),
	xpOnDeath(copy->xpOnDeath),
	attackCooldown(copy->attackCooldown)
{}


Enemy::~Enemy()
{
	shortTermObjective = nullptr;

	animation = Animation();
}


bool Enemy::PreUpdate(float dt)
{
	return true;
}


bool Enemy::Update(float dt)
{
	ENEMY_STATES current_state = ENEMY_STATES::UNKNOWN;
	//current_animation = &idle;

	//check inputs to traverse state matrix
	externalInput(inputs, dt);
	internalInput(inputs, dt);
	state = processFsm(inputs);

	if (state != current_state)
	{
		switch (state)
		{
		case ENEMY_STATES::IDLE:

			break;

		case ENEMY_STATES::MOVE:
			Move();
			break;

		case ENEMY_STATES::ATTACK:
			Attack();
			attackCooldown += dt;
			break;

		case ENEMY_STATES::CHARGING_ATTACK:
			break;

		case ENEMY_STATES::DEAD:
			Die();
			break;
		}
	}
	current_state = state;

	
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

	inputs.push_back(ENEMY_INPUTS::IN_MOVE);
	return true;
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


void Enemy::Move()
{
	//Put logic to move the unit to the desired destination
}


void Enemy::Attack()
{
	//i have to think about this one

}


void Enemy::Die()
{
	toDelete = true;
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
	static ENEMY_STATES state = ENEMY_STATES::IDLE;
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