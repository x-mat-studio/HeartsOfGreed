#include "Entity.h"

Entity::Entity() 
{}


Entity::Entity(std::vector<int>& position, ENTITY_TYPES type, SDL_Texture* texture) :

	position(position),
	type(type),
	texture(texture),
	started(false),
	toDelete(false)
{}


Entity::~Entity()
{
	position.clear();

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
	return nullptr;
}


void Entity::Draw(float dt)
{
}