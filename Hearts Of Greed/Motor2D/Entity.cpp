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
	dynamic(dynamic),
	visionEntity(nullptr)
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
	dynamic(dynamic),
	visionEntity(nullptr)
{}

Entity::~Entity()
{
	texture = nullptr;
	collider->to_delete = true;
	collider = nullptr;
	if (visionEntity != nullptr && visionEntity!= NULL)
	{
		visionEntity->deleteEntity = true;
	}
	visionEntity = nullptr;
	
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
	
}

void Entity::CollisionPosUpdate()
{
	collider->SetPos(position.x - offset.x, position.y - offset.y);
}


Collider* Entity::GetCollider() const
{
	return collider;
}



void Entity::Draw(float dt)
{}


fMPoint Entity::GetPosition()
{
	return position;
}


void Entity::SetPosition(int x, int y)
{
	position.x = x;
	position.y = y;
}


void Entity::SetTexture(SDL_Texture* tex)
{
	texture = tex;
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
		switch (type)
		{
		case ENTITY_TYPE::HERO_MELEE:
			align = ENTITY_ALIGNEMENT::PLAYER;
			break;

		case ENTITY_TYPE::HERO_RANGED:
			align = ENTITY_ALIGNEMENT::PLAYER;
			break;

		case ENTITY_TYPE::HERO_GATHERER:
			align = ENTITY_ALIGNEMENT::PLAYER;
			break;

		case ENTITY_TYPE::ENEMY:
			align = ENTITY_ALIGNEMENT::ENEMY;
			break;

		default:
			align = ENTITY_ALIGNEMENT::NEUTRAL;
			break;
		}

	}
	else
	{
		align = newAlign;
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