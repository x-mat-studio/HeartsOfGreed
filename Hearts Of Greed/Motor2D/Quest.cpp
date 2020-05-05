#include "Quest.h"

Quest::Quest() :

	myState(QUEST_STATE::ST_UNKNOWN)
{}

void Quest::OnCollision(Collider * collider)
{
	this->myState = QUEST_STATE::FINISHED;
}
