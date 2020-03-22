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


void Module::OnCollision(Collider*, Collider*)
{
}


void Module::AddEvent(EVENT_ENUM& eventId)
{

	listener.push_back(eventId);

}


void Module::RemoveEvent(EVENT_ENUM& eventId)
{

	int numElem = listener.size();

	for (int i = 0; i < numElem; i++)
	{

		if (listener[i] == eventId)
		{
			listener.erase(listener.begin() + numElem);
		}

	}

}


bool Module::CheckListener()
{
	int numElem = listener.size();

	if (listener.size() == 0)
	{
		return false;
	}

	for (int i = 0; i < numElem; i++)
	{
		ExecuteEvent(listener[i]);
	}

	return true;
}


void Module::ExecuteEvent(EVENT_ENUM& eventId) const
{}

