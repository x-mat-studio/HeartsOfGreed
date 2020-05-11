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
	void BlitMyAnimation(float dt);

	int GetId();
	void SetId(int i);

private:

	void RemoveFromOngoing();

private:

	int id;
	//int queuePos;
	QUEST_STATE myState;

	

}; 

#endif //__QUEST_H__
