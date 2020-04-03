#include "Entity.h"
#include "Render.h"
#include "App.h"

Entity::Entity()
{}


Entity::Entity(fMPoint position, ENTITY_TYPE type, bool dynamic) :

	position(position),
	type(type),
	started(false),
	toDelete(false),
	collider(nullptr),
	flip(false),
	texture(nullptr),
	offset {0,0},
	dynamic(dynamic)
{}


Entity::Entity(fMPoint position, ENTITY_TYPE type, Collider* collider, bool dynamic) :

	position(position),
	type(type),
	started(false),
	toDelete(false),
	collider(collider),
	flip(false),
	texture(nullptr),
	offset{ 0,0 },
	dynamic(dynamic)
{}

Entity::~Entity()
{
	texture = nullptr;
	collider->to_delete = true;
	collider = nullptr;
}


bool Entity::Start(SDL_Texture* texture)
{
	this->texture = texture;

	if (collider != nullptr)
	{
		collider = new Collider(collider->rect, collider->type, collider->callback, this);
		collider->thisEntity = this;
		app->coll->AddColliderEntity(collider);
	}

	SetAlignment();

	started = true;

	offset.x =  (float)collider->rect.w* 0.5f;
	offset.y =  (float)collider->rect.h;

	return true;
}


bool Entity::PreUpdate(float dt)
{
	return true;
}


bool Entity::Update(float dt)
{
	return true;
}


bool Entity::PostUpdate(float dt)
{
	return true;
}


void Entity::OnCollision(Collider* collider)
{
	LOG("Yeeet :D");
}

void Entity::CollisionPosUpdate()
{
	collider->SetPos(position.x - offset.x, position.y - offset.y);
}


Collider* Entity::GetCollider() const
{
	return collider;
}



void Entity::Draw()
{}


fMPoint Entity::GetPosition()
{
	return position;
}


ENTITY_TYPE Entity::GetType()
{
	return type;
}

ENTITY_ALIGNEMENT Entity::GetAlignment()
{
	return this->align;
}

void Entity::SetAlignment(ENTITY_ALIGNEMENT newAlign)
{
	if (newAlign == ENTITY_ALIGNEMENT::UNKNOWN)
	{
		switch (this->type)
		{
		case ENTITY_TYPE::HERO_MELEE:
			this->align = ENTITY_ALIGNEMENT::PLAYER;
			break;

		case ENTITY_TYPE::HERO_RANGED:
			this->align = ENTITY_ALIGNEMENT::PLAYER;
			break;

		case ENTITY_TYPE::HERO_GATHERER:
			this->align = ENTITY_ALIGNEMENT::PLAYER;
			break;

		case ENTITY_TYPE::ENEMY:
			this->align = ENTITY_ALIGNEMENT::ENEMY;
			break;

		default:
			this->align = ENTITY_ALIGNEMENT::NEUTRAL;
			break;
		}

	}
	else
	{
		this->align = newAlign;
	}

}

void Entity::SetToDelete(bool toDel)
{
	if (toDel != toDelete)
	{
		toDelete = !toDelete;
	}
}


bool Entity::DealDamage(int damage)
{

	return false;
}