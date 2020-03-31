#ifndef __ASSETMANAGER_H__
#define __ASSETMANAGER_H__

#include "Module.h"

#include "PhysFS/include/physfs.h"
#include "SDL/include/SDL.h"

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

class ModuleAssetManager : public Module
{
public:

	ModuleAssetManager();
	~ModuleAssetManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Return the bytes of a PhysFS filehandle
	uint Load(const char* path, char** buffer) const;

	// Allows you to use pointers to memory instead of files or things such as images or samples
	SDL_RWops* Load(const char* path) const;

};

#endif // __ASSETMANAGER_H__
