#include "p2Defs.h"
#include "p2Log.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )


ModuleTextures::ModuleTextures() : Module()
{
	name.create("textures");
}


// Destructor
ModuleTextures::~ModuleTextures()
{}


// Called before render is available
bool ModuleTextures::Awake(pugi::xml_node& config)
{
	LOG("Init Image library");
	bool ret = true;
	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);


	if ((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}


// Called before the first frame
bool ModuleTextures::Start()
{
	LOG("start textures");
	bool ret = true;
	return ret;
}


// Called before quitting
bool ModuleTextures::CleanUp()
{
	LOG("Freeing textures and Image library");
	int numTextures = textures.size();

	for (int i = 0; i < numTextures; i++)
	{
		SDL_DestroyTexture(textures[i]);
	}

	textures.clear();
	IMG_Quit();
	return true;
}


// Load new texture from file path
SDL_Texture* const ModuleTextures::Load(const char* path)
{
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load(path);


	if (surface == NULL)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
	}
	else
	{
		texture = LoadSurface(surface);
		SDL_FreeSurface(surface);
	}


	return texture;
}


// Unload texture
bool ModuleTextures::UnLoad(SDL_Texture* texture)
{
	int numTextures = textures.size();


	for (int i = 0; i < numTextures; i++)
	{


		if (texture == textures[i])
		{
			SDL_DestroyTexture(textures[i]);
			textures.erase(textures.begin() + i);
			return true;
		}


	}


	return false;
}


// Translate a surface into a texture
SDL_Texture* const ModuleTextures::LoadSurface(SDL_Surface* surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(app->render->renderer, surface);


	if (texture == NULL)
	{
		LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError()); //TODO solve this
	}
	else
	{
		textures.push_back(texture);
	}


	return texture;
}


// Retrieve size of a texture
void ModuleTextures::GetSize(const SDL_Texture* texture, uint& width, uint& height) const
{
	SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, (int*)& width, (int*)& height);
}
