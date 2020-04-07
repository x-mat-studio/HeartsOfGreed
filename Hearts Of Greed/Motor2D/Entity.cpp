#include "Entity.h"
#include "Render.h"
#include "Window.h"
#include "Audio.h"
#include "App.h"

Entity::Entity()
{}


Entity::Entity(fMPoint position, ENTITY_TYPE type, ENTITY_ALIGNEMENT alignement, Collider* collider, bool dynamic) :

	position(position),
	type(type),
	align(alignement),
	dynamic(dynamic),

	started(false),
	toDelete(false),
	flip(false),
	
	collider(collider),
	visionEntity(nullptr),
	texture(nullptr),

	offset {0, 0}
{}

Entity::~Entity()
{
	collider->to_delete = true;

	texture = nullptr;
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

		collider->SetPos(position.x, position.y);
	}

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


DIRECTION Entity::GetMyDirection()
{
	int midScreenX = app->render->GetCameraX() + app->win->width / 2;
	
	int relativeX = position.x - midScreenX;
	

	if (relativeX > 30) {

		return DIRECTION::RIGHT;
	}

	if (relativeX < -30) {

		return DIRECTION::LEFT;
	}
	
	return DIRECTION::FRONT;
}


LOUDNESS Entity::GetMyLoudness()
{
	int width = app->win->width; int height = app->win->height;

	int MidX = (-app->render->GetCameraX() + width  /2);
	int MidY = (-app->render->GetCameraY() + height /2);

	float SQRDistance = sqrt((position.x - MidX) * (position.x - MidX) + (position.y - MidY) * (position.y - MidY));

	if (SQRDistance < width/4 * app->win->GetScale()) {

		return LOUDNESS::LOUD;
	}
	if (SQRDistance < width/3 * app->win->GetScale()) {

		return LOUDNESS::NORMAL;
	}
	if (SQRDistance < width/2 * app->win->GetScale()) {

		return LOUDNESS::QUIET;
	}
	

	return LOUDNESS::SILENCE;

}



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