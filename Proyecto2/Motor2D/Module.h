// ----------------------------------------------------
// Module.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __MODULE_H__
#define __MODULE_H__

#define MAX_COLLIDERS_PER_FRAME 50
#define MAX_LISTENERS 10

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

class Event {}; //PLACEHOLDER juan pls

class Listener
{
public:

	virtual ~Listener() {}

	virtual void onNotify(const Module& module, Event event) = 0;

private:

	//We do not need to remove listeners from speakers: we never delete modules on runtime

};

class Speaker 
{
private:

	Listener* listeners_[MAX_LISTENERS]; //Array, we're not expecting to add/remove a lot


	int numListeners;


	bool AddListener(Listener* listener) {

		if (numListeners < MAX_LISTENERS) {

			numListeners++;

			for (int i = 0; i < MAX_LISTENERS; i++) {

				if (listeners_[i] == nullptr) {

					listeners_[i] = listener;

					return true;
				}
			}
		}

		return false;
	}


	bool RemoveListener(Listener* listener) {
		
		for (int i = 0; i < numListeners; i++) {
		
			if (listeners_[i] == listener)
			{
				listeners_[i] = nullptr;

				numListeners--;

				return true;
			}
		
		}
	
		return false;
	}


protected:

	void Speak(const Module& module, Event event)
	{
		for (int i = 0; i < numListeners; i++)
		{
			listeners_[i]->onNotify(module, event);
		}
	}
};




#endif // __MODULE_H__