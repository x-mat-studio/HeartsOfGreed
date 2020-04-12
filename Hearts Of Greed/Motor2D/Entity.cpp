#include "Entity.h"
#include "Render.h"
#include "Window.h"
#include "Audio.h"
#include "App.h"
#include "Minimap.h"

Entity::Entity()
{}


Entity::Entity(fMPoint position, ENTITY_TYPE type, ENTITY_ALIGNEMENT alignement, Collider* collider, int maxHealth, int currentHealth, bool dynamic) :

	position(position),
	type(type),
	align(alignement),
	dynamic(dynamic),

	started(false),
	toDelete(false),
	flip(false),
	
	collider(collider),
	visionEntity(nullptr),
	minimapIcon(nullptr),
	texture(nullptr),

	hitPointsMax(maxHealth),
	hitPointsCurrent(currentHealth),

	offset {0, 0},

	UIAssigned(false)
{
}

Entity::~Entity()
{
	if (collider != nullptr)
	{
		collider->to_delete = true;
		collider->thisEntity = nullptr;
	}
	

	texture = nullptr;
	collider = nullptr;

	if (visionEntity != nullptr)
	{
		visionEntity->deleteEntity = true;
	}

	minimapIcon = nullptr;
}


bool Entity::Start(SDL_Texture* texture)
{
	this->texture = texture;

	int w;
	int h;

	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	center.x = w / 2;
	center.y = h / 2;

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

void Entity::MinimapDraw(float scale, float halfWidth)
{

}


DIRECTION Entity::GetMyDirection()
{
	int width = app->win->width; 

	int MidX = (-app->render->GetCameraX() + width / 2);
	
	int relativeX = position.x - MidX;

	if (relativeX > 120) {

		return DIRECTION::RIGHT;
	}

	if (relativeX < -120) {

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


fMPoint Entity::GetCenter()
{
	return center;
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


SDL_Texture* Entity::GetTexture()
{
	return texture;
}


ENTITY_TYPE Entity::GetType()
{
	return type;
}


ENTITY_ALIGNEMENT Entity::GetAlignment()
{
	return this->align;
}

bool Entity::IsOpositeAlignement(ENTITY_ALIGNEMENT check)
{
	switch (this->align)
	{
	case ENTITY_ALIGNEMENT::PLAYER:
	{
		if (check == ENTITY_ALIGNEMENT::ENEMY)
			return true;

		return false;
	}
	break;
	case ENTITY_ALIGNEMENT::ENEMY:
	{
		if (check == ENTITY_ALIGNEMENT::PLAYER)
			return true;
	}
	break;
	case ENTITY_ALIGNEMENT::NEUTRAL:
	{
		return false;
	}
	break;

	}


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


int Entity::RecieveDamage(int damage)
{
	return -1;
}

