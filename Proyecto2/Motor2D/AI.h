#pragma once

#ifndef __AI_H__
#define __AI_H__

#include "Module.h"

class ModuleAI : public Module
{
public:

	ModuleAI();
	~ModuleAI();

	bool Awake(pugi::xml_node&);

	void OnCollision(Collider*, Collider*);

private:
	void CreateSelectionCollider(Collider*);
	
private:

};


#endif//__AI_H__
