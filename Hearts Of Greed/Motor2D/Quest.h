#ifndef __QUEST_H__
#define __QUEST_H__

#include "Entity.h"

enum class QUEST_STATE : int
{
	ST_UNKNOWN = -1,

	ACTIVE,
	FINISHED,

	ST_ALL,
};


class Quest : public Entity
{

public:
	Quest();

	void OnCollision(Collider* collider);

	int id;
	QUEST_STATE myState;

}; 

#endif //__QUEST_H__
