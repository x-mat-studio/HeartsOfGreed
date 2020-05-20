#ifndef __VIDEO_H__
#define __VIDEO_H__

#include "Module.h"

//Video player for windows libraries
#include <dshow.h>
#include <Vfw.h>                          
#pragma comment(lib,"vfw32.lib")               


class Video : public Module
{
public:

	Video();

	// Destructor
	virtual ~Video();
	bool Update(float dt);

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	void Initialize(char* path);
	void OpenAVI(char* path);
	bool GrabAVIFrame();

	void CloseAVI();

public:
	bool				isVideoFinished = false;

private:
	// Frame Counter
	int			frame = 0;

	AVISTREAMINFO       psi;                        // Pointer To A Structure Containing Stream Info
	PAVISTREAM			pavi;                       // Handle To An Open Stream
	PGETFRAME			pgf;                        // Pointer To A GetFrame Object

	long				lastFrame;                  // Last Frame Of The Stream
	int					width;                      // Video Width
	int					height;                     // Video Height
	char*				pdata;					    // Pointer To Texture Data
	int					mpf;                        // Will Hold Rough Milliseconds Per Frame

	int i = 1;
};

#endif // __Video_H__
