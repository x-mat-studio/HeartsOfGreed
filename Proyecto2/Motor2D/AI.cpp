#include "AI.h"
#include "Collision.h"

ModuleAI::ModuleAI() : Module()
{
	name.create("player");
}


ModuleAI::~ModuleAI()
{}


bool ModuleAI::Awake(pugi::xml_node& config)
{

	return true;
}


void ModuleAI::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_BASE_ALERT && c2->type == COLLIDER_HERO)
	{
		CreateSelectionCollider(c1);
	}


}


void ModuleAI::CreateSelectionCollider(Collider* collider)
{
	
}