#ifndef __RENDER_H__
#define __RENDER_H__

#include "SDL/include/SDL.h"
#include "Module.h"
#include "p2Point.h"


class ModuleRender : public Module
{
public:

	ModuleRender();

	// Destructor
	virtual ~ModuleRender();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Blit
	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();
	bool Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, bool fliped = false, float pivotX = 0, float pivotY = 0, float speedX = 1.0f, float speedY = 1.0f, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX);
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true);
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true);
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true);

	// Set background color
	void SetBackgroundColor(SDL_Color color);

	//Returns the width & height of the camera
	void GetCameraMeasures(int& w, int& h);

	const int GetCameraX();
	const int GetCameraY();

public:
	SDL_Renderer* renderer;

	SDL_Rect viewport;

	SDL_Color background;

	float currentCamX;//buffer for the camera movement(otherwise when converting to int behaves wrong)
	float currentCamY;//buffer for the camera movement(otherwise when converting to int behaves wrong)
private:
	SDL_Rect camera;

};

#endif // __RENDER_H__