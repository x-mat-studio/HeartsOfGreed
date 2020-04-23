#ifndef __FONTS_H__
#define __FONTS_H__

#include "Module.h"
#include "SDL\include\SDL_pixels.h"
#include "SDL\include\SDL.h"
#include "SDL_TTF/include/SDL_ttf.h"

#pragma comment( lib, "SDL_ttf/libx86/SDL2_ttf.lib" )

#define DEFAULT_FONT "fonts/jh_fallout.ttf"
#define DEFAULT_FONT_SIZE 8

struct SDL_Texture;

class ModuleFonts : public Module
{
public:

	ModuleFonts();

	// Destructor
	virtual ~ModuleFonts();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Load Font
	TTF_Font* const Load(const char* path, int size = 12);

	// Create a surface from text
	SDL_Texture* Print(const char* text, SDL_Color color = { 255, 255, 255, 255 }, TTF_Font* font = NULL);

	bool CalcSize(const char* text, int& width, int& height, TTF_Font* font = NULL) const;

private:

	SDL_Color GetPixelColor(const SDL_Surface* pSurface, const int X, const int Y);
	void SetPixelColor(SDL_Surface* pSurface, int X, int Y, Uint32 Pixel);


public:

	int ingameSize;

	std::vector<TTF_Font*>	fonts;
	TTF_Font* default;
};


#endif // __j1FONTS_H__
