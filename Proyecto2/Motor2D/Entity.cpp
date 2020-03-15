#include "Entity.h"
#include "App.h"

Entity::Entity()
{}


Entity::Entity(SDL_Point position, ENTITY_TYPE type, SDL_Texture* texture) :

	position(position),
	type(type),
	texture(texture),
	started(false),
	toDelete(false)
{}


Entity::Entity(SDL_Point position, ENTITY_TYPE type, SDL_Texture* texture, SDL_Rect collRect, COLLIDER_TYPE collType, Module* callback) :

	position(position),
	type(type),
	texture(texture),
	started(false),
	toDelete(false)
{
	collider = new Collider(collRect, collType, callback, this);
	app->coll->AddColliderEntity(collider);

}

Entity::~Entity()
{
	texture = nullptr;
}


bool Entity::Start()
{
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


Collider* Entity::GetCollider() const
{
	return collider;
}


void Entity::Draw(float dt)
{
}


SDL_Point Entity::GetPosition()
{
	return position;
}


ENTITY_TYPE Entity::GetType()
{
	return type;
}