#include "Entity.h"
#include "Render.h"
#include "Window.h"
#include "Audio.h"
#include "App.h"
#include "Minimap.h"
#include "Map.h"

Entity::Entity() : position(position),
	type(ENTITY_TYPE::UNKNOWN),
	align(ENTITY_ALIGNEMENT::UNKNOWN),
	dynamic(false),

	started(false),
	toDelete(false),
	flip(false),
	selectedByPlayer(false),
	missionEntity(false),
	UIAssigned(false),

	collider(nullptr),

	visionEntity(nullptr),
	minimapIcon(nullptr),
	texture(nullptr),

	hitPointsMax(0),
	hitPointsCurrent(0),

	offset{ 0, 0 },
	center{ 0, 0 },

	radiusSize(0)
{}


Entity::Entity(fMPoint position, ENTITY_TYPE type, ENTITY_ALIGNEMENT alignement, Collider* collider, int maxHealth, int currentHealth, bool dynamic) :

	position(position),
	type(type),
	align(alignement),
	dynamic(dynamic),

	started(false),
	toDelete(false),
	flip(false),
	selectedByPlayer(false),
	missionEntity(false),
	UIAssigned(false),

	collider(collider),

	visionEntity(nullptr),
	minimapIcon(nullptr),
	texture(nullptr),

	hitPointsMax(maxHealth),
	hitPointsCurrent(currentHealth),

	offset{ 0, 0 },
	center{ 0, 0 },

	radiusSize(1)

{
	if (collider != nullptr)
	{
		offset.x = -((float)collider->rect.w * 0.5f);

		offset.y = -((float)collider->rect.h * 0.66f);
	}
}

Entity::~Entity()
{
	if (collider != nullptr)
	{
		collider->to_delete = true;
		collider->thisEntity = nullptr;
		collider = nullptr;
	}


	texture = nullptr;

	visionEntity = nullptr;

	minimapIcon = nullptr;

	selectedByPlayer = false;
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

		offset.x = -((float)collider->rect.w * 0.5f);

		offset.y = -((float)collider->rect.h * 0.66f);

		center.x = (float)collider->rect.w * 0.5f;
		center.y = (float)collider->rect.h * 0.5f;

		CollisionPosUpdate();
	}
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
	collider->SetPos(position.x + offset.x, position.y + offset.y);
}


Collider* Entity::GetCollider() const
{
	return collider;
}

void Entity::SetColliderRect(SDL_Rect newRect)
{
	this->collider->rect = newRect;
}

void Entity::ActiveCollider()
{
	this->collider->Activate();
}


void Entity::Draw(float dt)
{}

void Entity::MinimapDraw(float scale, float halfWidth)
{}


DIRECTION Entity::GetMyDirection()
{
	int width = app->win->width;
	float scale = app->win->GetScale();
	int MidX = (-app->render->currentCamX + (width * 0.5f)) / scale;

	int relativeX = (position.x - MidX) * scale;

	if (relativeX > width * 0.33f) {

		return DIRECTION::RIGHT;
	}

	if (relativeX < -width * 0.33f) {

		return DIRECTION::LEFT;
	}

	return DIRECTION::FRONT;
}


LOUDNESS Entity::GetMyLoudness()
{
	int ret = LOUDNESS::SILENCE;

	float scale = app->win->GetScale();
	int width = app->win->width;
	int height = app->win->height;

	int MidX = (-app->render->currentCamX + (width * 0.5f)) / scale;
	int MidY = (-app->render->currentCamY + (height * 0.5f)) / scale;

	float SQRDistance = sqrt((position.x - MidX) * (position.x - MidX) + (position.y - MidY) * (position.y - MidY));

	if (SQRDistance < width * 0.25f)
	{

		ret = LOUDNESS::LOUD;
	}
	else if (SQRDistance < width * 0.33f)
	{

		ret = LOUDNESS::NORMAL;
	}
	else if (SQRDistance < width * 0.5f)
	{

		ret = LOUDNESS::QUIET;
	}

	//distance from camera loudness
	float minScale;
	float maxScale;
	app->win->GetScaleRange(minScale, maxScale);
	float scaleRange = abs(maxScale - minScale);


	if (scale >= minScale + (scaleRange * 0.66f))
	{
		ret -= 1;
	}
	else if (scale <= minScale + (scaleRange * 0.33f))
	{
		ret += 1;
	}

	ret = MAX(LOUDNESS::LOUD, ret);
	ret = MIN(LOUDNESS::SILENCE, ret);

	return (LOUDNESS)ret;
}



fMPoint Entity::GetPosition()
{
	return position;
}

int Entity::GetRadiusSize()
{
	return radiusSize;
}


fMPoint Entity::GetCenter()
{
	return center;
}

fMPoint Entity::GetOffset()
{
	return  offset;
}

void Entity::SetPosition(int x, int y)
{
	position.x = x;
	position.y = y;
}

int Entity::GetMaxHP() const
{
	return hitPointsMax;
}

void Entity::SetMaxHP(int newMaxHp)
{
	hitPointsMax = newMaxHp;
	
}

int Entity::GetCurrentHP() const
{
	return hitPointsCurrent;
}

void Entity::SetCurrentHP(int newcurrHp)
{
	hitPointsCurrent = newcurrHp;

	if (hitPointsCurrent > hitPointsMax)
		hitPointsCurrent = hitPointsMax;
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

		return false;
	}
	break;
	case ENTITY_ALIGNEMENT::NEUTRAL:
	{
		return false;
	}
	break;

	}

	return false;
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

		if (newAlign == ENTITY_ALIGNEMENT::ENEMY)
		{
			if (visionEntity != nullptr)
			{
				visionEntity->SetEntityProvideVision(false);
			}

			if (this->type == ENTITY_TYPE::BLDG_TURRET)
			{
				
					if (this->minimapIcon != nullptr)
					{
						this->minimapIcon->type = MINIMAP_ICONS::ENEMY_TURRET;
					}
				
			}
		}
		else if (newAlign == ENTITY_ALIGNEMENT::PLAYER)
		{
			if (visionEntity != nullptr)
			{
				visionEntity->SetEntityProvideVision(true);
			}

			if (this->type == ENTITY_TYPE::BLDG_TURRET)
			{

				if (this->minimapIcon != nullptr)
				{
					this->minimapIcon->type = MINIMAP_ICONS::TURRET;
				}

			}
		}
	}

}


void Entity::SetToDelete(bool toDel)
{
	if (toDel != toDelete)
	{
		toDelete = !toDelete;
	}
}


int Entity::RecieveDamage(float damage, bool ignoreArmor)
{
	return -1;
}

void Entity::CheckObjective(Entity* deleted)
{
	return;
}

void Entity::DebugDraw()
{
	if (!app->debugMode)
	{
		return;
	}

	//Position --------------------------------------
	app->render->DrawQuad({ (int)position.x, (int)position.y, 2,2 }, 255, 0, 0);

	fMPoint nextPoint = { 0,0 };
	iMPoint origin = app->map->WorldToMap(round(position.x), round(position.y));
	origin = app->map->MapToWorld(origin.x, origin.y);

	app->render->DrawQuad({ (int)origin.x, (int)origin.y, 10,10 }, 255, 255, 255, 125);

}

void Entity::ApplyEffect()
{
	return;
}
