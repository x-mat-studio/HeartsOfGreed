#ifndef __CAMERA_SHAKE_H__
#define __CAMERA_SHAKE_H__

#include "Module.h"

class ModuleCameraShake : public Module
{
public:
	ModuleCameraShake();
	~ModuleCameraShake();

	bool PostUpdate(float dt);

	bool CleanUp();

	void StartCameraShake(float time, int magnitude);
	fMPoint GetCameraOffset();


private:
	void ShakeCamera(float dt);


private:
	bool shaking;
	float shakeTotalTime;
	float currentTime;

	int magnitude;

	fMPoint cameraOffset;
};


#endif //__CAMERA_SHAKE_H__

