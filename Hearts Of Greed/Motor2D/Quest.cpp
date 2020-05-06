#include "App.h"
#include "Quest.h"
#include "EventManager.h"
#include "Collision.h"
#include "QuestManager.h"

Quest::Quest() :

	myState(QUEST_STATE::ACTIVE)
{}

Quest::Quest(int x, int y) : 
	Entity(position, ENTITY_TYPE::QUEST, ENTITY_ALIGNEMENT::NEUTRAL, nullptr, 1, 1)
{
	SDL_Rect auxQ{ x, y,20,20 };
	this->collider = app->coll->AddCollider(auxQ, COLLIDER_QUEST,app->questManager,this);
	this->id = 0;
	this->position = { (float)x,(float)y };
	//this.id = check size of the lists and shit
}

Quest::Quest(Collider * col):

	myState(QUEST_STATE::ACTIVE),
	Entity(position, ENTITY_TYPE::QUEST, ENTITY_ALIGNEMENT::NEUTRAL, col, 1, 1)
{
	this->id = 0;
	//this.id = check size of the lists and shit
}

void Quest::OnCollision(Collider * collider)
{
	this->myState = QUEST_STATE::FINISHED; 

	app->eventManager->GenerateEvent(EVENT_ENUM::FINISH_QUEST, EVENT_ENUM::NULL_EVENT);

	this->collider->to_delete = true;
}