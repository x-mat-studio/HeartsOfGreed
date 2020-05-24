#ifndef __QUEST_H__
#define __QUEST_H__

#include "Entity.h"

enum class QUEST_STATE : int
{
	ST_UNKNOWN = -1,
	
	INACTIVE,
	ACTIVE,

	ST_ALL,
};


class Quest : public Entity
{

public:
	Quest(int x, int y);
	Quest(Collider* col);

	~Quest();

	void Draw(float dt);

	void OnCollision(Collider* collider);
	void BlitMyAnimation(float dt);

	int GetId() const;
	void SetId(int i);

private:

	int id;
	//int queuePos;
	QUEST_STATE myState;

	

}; 

#endif //__QUEST_H__
