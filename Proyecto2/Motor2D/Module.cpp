#include "Module.h"

Module::Module() : active(false)
{}

void Module::Init()
{
	active = true;
}


bool Module::Awake(pugi::xml_node&)
{
	return true;
}


bool Module::Start()
{
	return true;
}


bool Module::PreUpdate(float dt)
{
	return true;
}


bool Module::Update(float dt)
{
	return true;
}


bool Module::PostUpdate(float dt)
{
	return true;
}


bool Module::CleanUp()
{
	return true;
}


bool Module::Load(pugi::xml_node&)
{
	return true;
}


bool Module::Save(pugi::xml_node&) const
{
	return true;
}


bool Module::IsEnabled() const
{
	return enabled;
}


void Module::Enable()
{

	if (enabled == false)
	{
		enabled = true;
		Start();
	}

}


void Module::Disable()
{

	if (enabled == true)
	{
		enabled = false;
		CleanUp();
	}

}


bool Module::AddEvent(EVENT_ENUM& eventId)
{

	for (int i = 0; i < MAX_LISTENERS; i++)
	{
		if (listener[i] == EVENT_ENUM::NULL_EVENT)
		{
			listener[i] = eventId;
			return true;
		}
	}
	return false;
}


bool Module::RemoveEvent(EVENT_ENUM& eventId)
{

	for (int i = 0; i < MAX_LISTENERS; i++)
	{

		if (listener[i] == eventId)
		{
			listener[i] = EVENT_ENUM::NULL_EVENT;
			return true;
		}

	}

	return false;
}

