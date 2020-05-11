#include "App.h"
#include "Quest.h"
#include "EventManager.h"
#include "Collision.h"
#include "QuestManager.h"
#include "Render.h"

Quest::Quest(int x, int y) :

	Entity(position, ENTITY_TYPE::QUEST, ENTITY_ALIGNEMENT::NEUTRAL, nullptr, 1, 1),
	myState(QUEST_STATE::ACTIVE)
{
	SDL_Rect auxQ{ x, y,128,128 };
	this->collider = app->coll->AddCollider(auxQ, COLLIDER_QUEST, app->questManager, this);

	this->id = app->questManager->ongoing.size() + app->questManager->finished.size() + 1;


	app->questManager->ongoing.insert({ id, this });

	this->position = { (float)x,(float)y };
	this->texture = app->questManager->questMarker;
}


Quest::Quest(Collider* col) :

	myState(QUEST_STATE::ACTIVE),
	Entity(position, ENTITY_TYPE::QUEST, ENTITY_ALIGNEMENT::NEUTRAL, col, 1, 1)
{
	this->collider = col;

	this->id = app->questManager->ongoing.size() + app->questManager->finished.size() + 1;
	app->questManager->ongoing.insert({ id, this });

	this->position = { (float)col->rect.x, (float)col->rect.y };
	this->texture = app->questManager->questMarker;

}


void Quest::Draw(float dt)
{

	if (this->myState == QUEST_STATE::ACTIVE) {
		app->render->Blit(texture, position.x, position.y, 0, false, true, 0, 255, 255, 255, 1.0f);
	}
	// blit my particle effect here
}


void Quest::OnCollision(Collider* collider)
{
	this->myState = QUEST_STATE::FINISHED;

	app->eventManager->GenerateEvent(EVENT_ENUM::FINISH_QUEST, EVENT_ENUM::NULL_EVENT);

	RemoveFromOngoing();
}

void Quest::RemoveFromOngoing()
{
	//toDelete = true;
	//app->eventManager->GenerateEvent(EVENT_ENUM::ENTITY_DEAD, EVENT_ENUM::NULL_EVENT);

	app->questManager->ongoing.erase(this->id);
	app->questManager->finished.insert({ id, this });

	collider->to_delete = true;
	collider->thisEntity = nullptr;

}

void Quest::BlitMyAnimation(float dt)
{

	//this shoould be a yellow particle effect

}


int Quest::GetId()
{
	return id;
}


void Quest::SetId(int i)
{
	id = i;
}