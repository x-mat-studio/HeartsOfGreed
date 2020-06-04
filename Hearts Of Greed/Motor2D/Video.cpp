#include "p2Log.h"
#include "App.h"
#include "AssetManager.h"
#include "Window.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Video.h"

#include "SDL_mixer\include\SDL_mixer.h"

Video::Video() :

	pavi(nullptr),               
	pgf(nullptr),
	lastFrame(NULL),
	width(NULL),
	height(NULL),
	pdata(NULL),
	mpf(NULL),
	psi()
{
	name.create("video");
}

Video::~Video()
{}

bool Video::Update(float dt)
{

	if (pavi != nullptr) 
	{
		if (!isVideoFinished)
		{
			GrabAVIFrame();
		}
		else
			Mix_PauseMusic();
	}


	if (isVideoFinished && pavi != nullptr) 
	{
		frame = 0;
		CloseAVI();
	}
		
	

	return true;
}

bool Video::Awake(pugi::xml_node&)
{
	return true;
}

bool Video::CleanUp()
{
	isVideoFinished = true;
	return true;
}

void Video::Initialize(char* path)
{
	OpenAVI(path);                  // Open The AVI File
	app->audio->PlayMusic("data/video/introOGG.ogg");
}

void Video::OpenAVI(char* path)
{
	if (AVIStreamOpenFromFile(&pavi, path, streamtypeVIDEO, 0, OF_READ, NULL) != 0) // Opens The AVI Stream
		LOG("Failed To Open The AVI Stream");

	
	AVIStreamInfo(pavi, &psi, sizeof(psi));					// Reads Information About The Stream Into psi
	width = (psi.rcFrame.right - psi.rcFrame.left);           // Width Is Right Side Of Frame Minus Left
	height = (psi.rcFrame.bottom - psi.rcFrame.top);          // Height Is Bottom Of Frame Minus Top
	lastFrame = AVIStreamLength(pavi);						// The Last Frame Of The Stream


	pgf = AVIStreamGetFrameOpen(pavi, (LPBITMAPINFOHEADER)AVIGETFRAMEF_BESTDISPLAYFMT);              // Create The PGETFRAME Using Our Request Mode
	if (pgf == NULL)
		LOG("Failed To Open The AVI Frame");

	isVideoFinished = false;
}

bool Video::GrabAVIFrame()
{

	LPBITMAPINFOHEADER lpbi;													// Holds The Bitmap Header Information
	lpbi = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pgf, frame);					// Grab Data From The AVI Stream
	pdata = (char*)lpbi + lpbi->biSize + lpbi->biClrUsed * sizeof(RGBQUAD);    // Pointer To Data Returned By AVIStreamGetFrame
																				// (Skip The Header Info To Get To The Data)
																				// Convert Data To Requested Bitmap Format


	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pdata, width, height, lpbi->biBitCount, width * 3, 0, 0, 0, 0);
	SDL_Texture* texture = app->tex->LoadSurface(surface);

	SDL_Rect camera = { 0, 0, width, height};
	SDL_Rect position = { ( width / 2) * 1.8, ( height / 2) + 50, width * 2, height * 2};
	SDL_Point point = { width / 2, height / 2 };

	SDL_RenderCopyEx(app->render->renderer, texture, &camera, &position, 180, &point, SDL_FLIP_HORIZONTAL);

	if (i % 2 == 0)
	{
		frame++;
	}
	i++;
	if (frame >= lastFrame)
	{
		frame = 0;
		isVideoFinished = true;
	}


	app->tex->UnLoad(texture);
	SDL_FreeSurface(surface);

	return true;
}

void Video::CloseAVI()
{
	AVIStreamGetFrameClose(pgf);                // Deallocates The GetFrame Resources

	AVIStreamRelease(pavi);                     // Release The Stream

	AVIFileExit();                              // Release The File

	pavi = nullptr;
	pgf = nullptr;

}