#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Module.h"

struct SDL_Window;
struct SDL_Surface;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Changae title
	void SetTitle(const char* new_title);

	// Retrive window size
	void GetWindowSize(uint& width, uint& height) const;

	// Retrieve window scale
	float GetScale() const;

	float SetScale(float newScale);

	float  AddScale(float addedScale);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screenSurface;

	uint		width;
	uint		height;
	

private:
	P2SString	title;
	float		scale;
	float minScaleValue;
	float maxScaleValue;

};


class WindowListener : public Listener
{
private:

	void onNotify(const Module& module, Event event);

};


class WindowSpeaker : public Speaker
{
private:
	Listener* listeners_[MAX_LISTENERS]; //Array, we're not expecting to add/remove a lot

	int numListeners;

};
#endif // __WINDOW_H__