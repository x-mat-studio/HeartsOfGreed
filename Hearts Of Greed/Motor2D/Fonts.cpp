#include "App.h"
#include "Textures.h"
#include "Fonts.h"
#include "Window.h"


ModuleFonts::ModuleFonts() : Module(),
ingameSize(5)
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
	TTF_Font* font = TTF_OpenFont(path, size);

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
SDL_Texture* ModuleFonts::Print(const char* text, SDL_Color color, TTF_Font* font)
{
	SDL_Texture* ret = nullptr;

	if (font == nullptr)
		font = default;

	SDL_Surface* surface = TTF_RenderText_Blended((font) ? font : font, text, color);


	if (surface == nullptr || surface->pixels == nullptr)
	{
		LOG("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		ret = app->tex->LoadSurface(surface);
		SDL_FreeSurface(surface);
	}

	return ret;
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