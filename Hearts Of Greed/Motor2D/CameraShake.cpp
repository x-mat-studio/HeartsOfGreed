#include "App.h"
#include "CameraShake.h"
#include "Render.h"

ModuleCameraShake::ModuleCameraShake() :

	Module(),

	shakeTotalTime(0),
	currentTime(0),

	magnitude(0),

	cameraOffset(0, 0),

	shaking(false)
{}


ModuleCameraShake::~ModuleCameraShake()
{
}


bool ModuleCameraShake::PostUpdate(float dt)
{
	if (shaking == true && dt != 0)
	{
		ShakeCamera(dt);
		app->render->currentCamX += cameraOffset.x;
		app->render->currentCamY += cameraOffset.y;
	}

	return true;
}


bool ModuleCameraShake::CleanUp()
{
	shaking = false;

	shakeTotalTime = 0;
	currentTime = 0;

	magnitude = 0;

	cameraOffset = { 0, 0 };

	return true;
}


void ModuleCameraShake::StartCameraShake(float time, int mag)
{
	shaking = true;

	shakeTotalTime = time;
	currentTime = 0;
	magnitude = mag;

}


fMPoint ModuleCameraShake::GetCameraOffset()
{
	return cameraOffset;
}


void ModuleCameraShake::ShakeCamera(float dt)
{
	float x = 0.f;
	float y = 0.0f;

	if (currentTime < shakeTotalTime)
	{
		int xDir = rand() % 2;
		int yDir = rand() % 2;

		if (xDir == 0)
		{
			x = rand() % magnitude;
			cameraOffset.x = x;
		}

		else if (xDir == 1)
		{
			x = rand() % magnitude;
			cameraOffset.x = -x;
		}


		if (yDir == 0)
		{
			y = rand() % magnitude;
			cameraOffset.y = y;
		}

		else if (yDir == 1)
		{
			y = rand() % magnitude;
			cameraOffset.y = -y;
		}
		
		currentTime += dt;
	}

	else
	{
		shaking = false;
		cameraOffset = { 0,0 };
	}
}