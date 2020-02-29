// ----------------------------------------------------
// j1Module.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __j1MODULE_H__
#define __j1MODULE_H__

#define MAX_COLLIDERS_PER_FRAME 50

#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"

struct Collider; 

class j1App;

class UIElement;

class j1Module
{
public:

	j1Module() : active(false)
	{}

	void Init()
	{
		active = true;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}

public:

	virtual void OnCollision(Collider*, Collider*) {}
	virtual void ListenerUI(UIElement* UI_element) {}
	p2SString	name;
	bool		active;


	// Module activation 
	bool IsEnabled() const { return enabled; }

	void Enable()
	{
		if (enabled == false)
		{
			enabled = true;
			Start();
		}
	}

	void Disable()
	{
		if (enabled == true)
		{
			enabled = false;
			CleanUp();
		}
	}

	

private:
	bool enabled = true;
};

#endif // __j1MODULE_H__