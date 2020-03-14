// ----------------------------------------------------
// Module.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __MODULE_H__
#define __MODULE_H__

#define MAX_COLLIDERS_PER_FRAME 50

#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"

struct Collider;

class App;

class UIElement;

class Module
{
public:

	Module() : active(false)
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
	virtual bool PreUpdate(float dt)
	{
		return true;
	}


	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}


	// Called each loop iteration
	virtual bool PostUpdate(float dt)
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
	P2SString	name;
	bool		active;


	// Module activation 
	bool IsEnabled() const
	{
		return enabled;
	}


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

#endif // __MODULE_H__