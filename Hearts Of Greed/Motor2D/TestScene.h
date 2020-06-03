#ifndef __TEST_SCENE_H__
#define __TEST_SCENE_H__

#include "Module.h"
#include "EasingFunctions.h"

class ModuleTestScene : public Module
{
public:

	ModuleTestScene();

	// Destructor
	virtual ~ModuleTestScene();

	// Called before render is available
	bool Awake(pugi::xml_node&);
	
	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate(float dt);

	// Called each loop iteration
	bool Update(float dt);

	// Called each loop iteration
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();


	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void Zoom(float addZoomAmount, int windowTargetCenterX, int windowTargetCenterY, float currentScale);
	void StartDragging(iMPoint mousePos);
	void Drag(iMPoint mousePos, float scale);
	bool MouseCameraDisplacement(float camVel, float dt);

	void ToggleCamMovement();
	void ActivateCamMovement();
	void DeactivateCamMovement();
	bool GetCamMovementActivated() const;
	void ConstrainCameraToBorders();
	void NewCamEasing(fMPoint initialPos, fMPoint finalPos, float duration,EASING_TYPE easingType);
	bool IsCamDoingEasing()const;
	void UpdateCamEasing(float dt);
	void SetCamEasingState(bool active);
	void MoveCamTo(fMPoint worldPos, float duration, EASING_TYPE easing);

	//Returns the position of the center of the camera frame in world coordinates
	fMPoint CamToWorld();

	//Returns the postion of the cam if the given point were to be the center of the frame
	fMPoint WorldToCam(fMPoint worldPos);

	int GetDayNumber() const;
	bool IsNight() const;

	void GetTimer(int& min, int& sec);

	int GetNightRectAlpha() const;

private:
	void CalculateTimers(float dt);
	void ExecuteEvent(EVENT_ENUM eventId);
	void DrawNightRect(int alpha);

public:
	bool startFromLoad;
	bool haveJustBeenLoaded;

private:
	int prevMousePosX;
	int prevmousePosY;

	bool camUp;
	bool camDown;
	bool camRight;
	bool camLeft;
	bool camSprint;

	bool allowCamMovement;

	bool isNightTime;

	int dayNumber;

	int dayTimer;
	int nightTimer;

	float timer;

	bool menuScene;

	float camVel;
	fMPoint initialCamPos;

	iMPoint mapBordersUpperLeftCorner;
	iMPoint mapBordersBottomRightCorner;

	//this variable stores the number of pixels from the screen at which the camera can be moved by the mouse pos
	iMPoint camMarginMovements;

	float fadeTime;
	bool camToReset;
	float startingScale;
	bool mapLoaded;

	Easing camEasingX;
	Easing camEasingY;
	Easing nightRectAlpha;

};

#endif // __TEST_SCENE_H__