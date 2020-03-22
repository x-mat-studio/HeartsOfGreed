#include "Entity.h"
#include "Render.h"
#include "App.h"

Entity::Entity()
{}


Entity::Entity(iMPoint position, ENTITY_TYPE type, bool dynamic) :

	position(position),
	type(type),	
	started(false),
	toDelete(false),
	collider(nullptr),
	flip(false),
	current_animation(nullptr),
	texture(nullptr),
	dynamic(dynamic)
{}


Entity::Entity(iMPoint position, ENTITY_TYPE type, Collider* collider, bool dynamic) :

	position(position),
	type(type),	
	started(false),
	toDelete(false),
	collider(collider),
	flip(false),
	current_animation(nullptr),
	texture(nullptr),
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

	collider->thisEntity = this;
	app->coll->AddColliderEntity(collider);

	started = true;

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
	collider->SetPos(position.x, position.y);
}


Collider* Entity::GetCollider() const
{
	return collider;
}


void Entity::Draw(float dt)
{
	app->render->Blit(texture, position.x, position.y, &current_animation->GetCurrentFrameBox(dt));
}


iMPoint Entity::GetPosition()
{
	return position;
}


ENTITY_TYPE Entity::GetType()
{
	return type;
}

void Entity::SetToDelete(bool toDel)
{
	if (toDel != toDelete)
	{
		toDelete = !toDelete;
	}
}