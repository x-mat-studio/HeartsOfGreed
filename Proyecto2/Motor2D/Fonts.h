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
struct _TTF_Font;

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
	_TTF_Font* const Load(const char* path, int size = 12);

	// Create a surface from text
	SDL_Texture* Print(const char* text, SDL_Color color = { 255, 255, 255, 255 }, _TTF_Font* font = NULL);

	bool CalcSize(const char* text, int& width, int& height, _TTF_Font* font = NULL) const;


public:

	std::vector<_TTF_Font*>	fonts;
	_TTF_Font* default;
};


#endif // __j1FONTS_H__
