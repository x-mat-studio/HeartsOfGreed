#include "Entity.h"

Entity::Entity()
{}


Entity::Entity(SDL_Point position, ENTITY_TYPE type, SDL_Texture* texture) :

	position(position),
	type(type),
	texture(texture),
	started(false),
	toDelete(false)
{}


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