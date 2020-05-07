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
	Quest(int x, int y);
	Quest(Collider* col);

	void Draw(float dt);

	void OnCollision(Collider* collider);

	void Die();

	int id;
	QUEST_STATE myState;

	void BlitMyAnimation(float dt);

}; 

#endif //__QUEST_H__
