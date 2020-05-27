#include "App.h"
#include "Quest.h"
#include "EventManager.h"
#include "Collision.h"
#include "QuestManager.h"
#include "Render.h"

Quest::Quest(int x, int y) :

	Entity(fMPoint{(float)x,(float)y}, ENTITY_TYPE::QUEST, ENTITY_ALIGNEMENT::NEUTRAL, nullptr, 1, 1),
	myState(QUEST_STATE::INACTIVE),

	id(-1)
{
	SDL_Rect auxQ{ x, y,128,128 };
	collider = app->coll->AddCollider(auxQ, COLLIDER_QUEST, app->questManager, this);

	//texture = app->questManager->GetTexture(); //placeholder texture is now null due to alex request
}


Quest::Quest(Collider* col) :

	Entity(fMPoint(col->rect.x, col->rect.y), ENTITY_TYPE::QUEST, ENTITY_ALIGNEMENT::NEUTRAL, col, 1, 1),
	myState(QUEST_STATE::INACTIVE),

	id(-1)
{
	texture = app->questManager->GetTexture();
}


Quest::~Quest()
{
	id = -1;
	myState = QUEST_STATE::ST_UNKNOWN;
	minimapIcon->toDelete = true;
}



void Quest::Draw(float dt)
{

	if (this->myState == QUEST_STATE::INACTIVE) 
	{
		app->render->Blit(texture, position.x, position.y, 0, false, true, 0, 255, 255, 255, 1.0f);
	}
	// blit my particle effect here
}


void Quest::OnCollision(Collider* collider)
{
	this->myState = QUEST_STATE::ACTIVE;

	app->eventManager->GenerateEvent(EVENT_ENUM::ENTITY_DEAD, EVENT_ENUM::NULL_EVENT);


	
	app->questManager->QuestStarted(id);

	toDelete = true;
}


void Quest::BlitMyAnimation(float dt)
{


	//this shoould be a yellow particle effect
}

int Quest::GetId() const
{
	return id;
}

void Quest::SetId(int i)
{
	id = i;


	switch (id)
	{
	case 5:
		collider->rect = { collider->rect.x, collider->rect.y, 450,450 };
		break;

	case 6:
		collider->rect = { collider->rect.x, collider->rect.y, 450,450 };
		collider->Activate();
		break;

	}
}

