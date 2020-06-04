#include "App.h"
#include "AssetManager.h"
#include "Textures.h"
#include "Fonts.h"
#include "Window.h"
#include "SDL/include/SDL.h"



ModuleFonts::ModuleFonts() : Module(), default(nullptr),
ingameSize(6)
{
	name.create("fonts");
}

// Destructor
ModuleFonts::~ModuleFonts()
{
	default = nullptr;
}

// Called before render is available
bool ModuleFonts::Awake(pugi::xml_node& conf)
{
	LOG("Init True Type Font library");
	bool ret = true;

	if (TTF_Init() == -1)
	{
		LOG("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		ret = false;
	}
	else
	{
		const char* path = conf.child("default_font").attribute("file").as_string(DEFAULT_FONT);
		int size = conf.child("default_font").attribute("size").as_int(DEFAULT_FONT_SIZE);
		Load(path, size);
		Load(path, ingameSize);

		default = fonts[0];
	}
	if (default == nullptr)
		return false;


	return ret;
}

// Called before quitting
bool ModuleFonts::CleanUp()
{
	LOG("Freeing True Type fonts and library");
	int numFonts = fonts.size();

	for (int i = 0; i < numFonts; i++)
	{
		TTF_CloseFont(fonts[i]);
		LOG("SDL_ttf could not delete! SDL_ttf Error: %s\n", TTF_GetError());
		fonts[i] = nullptr;
	}
	fonts.clear();

	TTF_Quit();
	return true;
}

// Load new texture from file path
TTF_Font* const ModuleFonts::Load(const char* path, int size)
{
	TTF_Font* font = TTF_OpenFontRW(app->assetManager->Load(path), 1, DEFAULT_FONT_SIZE);


	if (font == NULL)
	{
		LOG("Could not load TTF font with path: %s. TTF_OpenFont: %s", path, TTF_GetError());
	}
	else
	{
		LOG("Successfully loaded font %s size %d", path, size);
		fonts.push_back(font);
	}

	return font;
}

// Print text using font
SDL_Texture* ModuleFonts::Print(const char* text, SDL_Color color, TTF_Font* font, Uint32 maxLenght)
{
	SDL_Texture* ret = nullptr;

	if (font == nullptr)
		font = default;

	SDL_Surface* surface = TTF_RenderUTF8_Blended_Wrapped((font) ? font : font, text, color, maxLenght);

	if (surface == nullptr || surface->pixels == nullptr || surface->format == nullptr)
	{
		LOG("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{

		ret = app->tex->LoadSurface(surface);

		if(surface != NULL)
			SDL_FreeSurface(surface);

	}


	return ret;
}


SDL_Color ModuleFonts::GetPixelColor(const SDL_Surface* pSurface, const int X, const int Y)
{
	const int Bpp = pSurface->format->BytesPerPixel;
	const Uint8* pPixel = (Uint8*)pSurface->pixels + Y * pSurface->pitch + X * Bpp;
	const Uint32 PixelColor = (Uint32)pPixel;

	SDL_Color Color = { 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE };

	const char* hola = SDL_GetPixelFormatName(pSurface->format->format);

	SDL_GetRGB(PixelColor, pSurface->format, &Color.r, &Color.g, &Color.b);

	return Color;
}

void ModuleFonts::SetPixelColor(SDL_Surface* pSurface, int X, int Y, Uint32 Pixel)
{
	Uint32* pPixels = (Uint32*)pSurface->pixels;

	pPixels[(Y * pSurface->w) + X] = Pixel;
}

// calculate size of a text
bool ModuleFonts::CalcSize(const char* text, int& width, int& height, TTF_Font* font) const
{
	if (font == NULL)
		font = default;

	bool ret = false;

	SDL_Surface* surface = TTF_RenderUTF8_Blended_Wrapped((font) ? font : font, text, { 255,255,255,255 }, app->win->width);

	if (surface)
	{
		width = surface->w;
		height = surface->h;
	}

	SDL_FreeSurface(surface);

	return ret;
}