#include "Entity.h"

Entity::Entity() 
{}


Entity::Entity(std::vector<int>& position, ENTITY_TYPES type, SDL_Texture* texture, Animation& animation) :
	position(position),
	type(type),
	texture(texture),
	animation(animation),
	started(false),
	toDelete(false)
{}


Entity::~Entity()
{
	position.clear();

	texture = nullptr;
	animation = Animation();
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