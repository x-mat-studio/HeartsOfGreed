#include "p2Defs.h"
#include "p2Log.h"
#include "Audio.h"
#include "Brofiler/Brofiler/Brofiler.h"
#include "SDL/include/SDL.h"
#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )


ModuleAudio::ModuleAudio() : Module(), music(NULL), volumeAdjustment(-27), musicVolume(180)
{
	name.create("audio");
}


// Destructor
ModuleAudio::~ModuleAudio()
{}


// Called before render is available
bool ModuleAudio::Awake(pugi::xml_node& config)
{
	BROFILER_CATEGORY("Audio Awake", Profiler::Color::AliceBlue);

	bool ret = true;
	SDL_Init(0);

	fxFolder.create(config.child("folder").child_value());


	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		enabled = false;
		ret = true;
	}


	// load support 
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);


	if ((init & flags) != flags)
	{
		enabled = false;
		ret = true;
	}


	//Initialize SDL_mixer
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		enabled = false;
		ret = true;
	}

	return ret;
}


// Called before quitting
bool ModuleAudio::CleanUp()
{
	if (!enabled)
		return true;


	if (music != NULL)
		Mix_FreeMusic(music);

	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}


// Play a music file
bool ModuleAudio::PlayMusic(const char* path, float fade_time, int volume)
{
	bool ret = true;

	if (volume < 0) {
		volume = 0;
	}
	Mix_VolumeMusic(volume);

	if (volume > 128)
	{
		musicVolume = 128;
	}
	else
	{
		musicVolume = volume;
	}

	if (!enabled)
		return false;


	if (music != NULL)
	{


		if (fade_time > 0.0f)
			Mix_FadeOutMusic(int(fade_time * 1000.0f));
		else
			Mix_HaltMusic();


		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}


	music = Mix_LoadMUS(path);


	if (music == NULL)
		ret = false;
	else
	{
		if (fade_time > 0.0f)
		{
			if (Mix_FadeInMusic(music, -1, (int)(fade_time * 1000.0f)) < 0)
				ret = false;
		}
		else
		{
			if (Mix_PlayMusic(music, -1) < 0)
				ret = false;
		}
	}


	return ret;
}


// Load WAV
unsigned int ModuleAudio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if (!enabled)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if (chunk == NULL)
	{

	}
	else
	{
		fx.push_back(chunk);
		ret = fx.size();
	}

	return ret;
}


// Play WAV
bool ModuleAudio::PlayFx(unsigned int id, int repeat, int channel, LOUDNESS loudness, DIRECTION direction, bool overrideChannel)
{
	id += 0;
	if (!enabled || loudness == LOUDNESS::SILENCE)
		return false;

	if(overrideChannel)
		Mix_HaltChannel(channel);

	if (channel > 0 && Mix_Playing(channel) == 0) {
		ConfigureChannel(channel, loudness, direction);
		if (id > 0 && id <= fx.size())
		{
			Mix_PlayChannel(channel, fx[id - 1], repeat, );
			//Input explanation: first argument is channel, -1  meanining first free channel. Second argument is a pointer to the chunk to play.
			//3rd argument is number of loops. if you want in once, put 0. -1 plays it "infinite" times.
		}
	}
	if (channel < 0) {

		channel = 1;

		while (Mix_Playing(channel) == 1) { 
			if (channel > 8) { return false; }
			channel++; 
		}
		ConfigureChannel(channel, loudness, direction);
		Mix_PlayChannel(channel, fx[id - 1], repeat, );
	}
	
	return true;
}

void ModuleAudio::SetVolume(float volume)
{
	float auxVolume = volume;

	if (auxVolume > 128)auxVolume = 128;
	if (auxVolume < 0)auxVolume = 0;

	musicVolume = auxVolume;
	Mix_VolumeMusic(auxVolume);
}

// Configure Audio Channel
bool ModuleAudio::ConfigureChannel(unsigned int channel, int volume, float angle)
{
	
	angle = -90;
	

	Mix_SetPosition(channel, angle, volume);
	
	return false;
}


// Configure Audio Channel for dummies
bool ModuleAudio::ConfigureChannel(unsigned int channel, LOUDNESS loudness, DIRECTION direction)
{
	float angle;
	int volume;
	bool ret = true;

	switch (loudness)
	{
	case LOUDNESS::QUIET:
		volume = 200 - volumeAdjustment;
		break;
	case LOUDNESS::NORMAL:
		volume = 100 - volumeAdjustment;
		break;
	case LOUDNESS::LOUD:
		volume = 1 - volumeAdjustment;
		break;
	default:
		Mix_HaltChannel(-1); //Someone missused the function
		volume = 0;
		angle = 0;
		ret = false;
		break;
	}
	
	switch (direction)
	{
	case DIRECTION::FRONT:
		angle = 0;
		break;
	case DIRECTION::FRONT_RIGHT:
		angle = 45;
		break;
	case DIRECTION::RIGHT:
		angle = 90;
		break;
	case DIRECTION::BACK_RIGHT:
		angle = 135;
		break;
	case DIRECTION::BACK:
		angle = 180;
		break;
	case DIRECTION::BACK_LEFT:
		angle = 225;
		break;
	case DIRECTION::LEFT:
		angle = 270;
		break;
	case DIRECTION::FRONT_LEFT:
		angle = 315;
		break;
	default:
		Mix_HaltChannel(-1); //Someone missused the function
		volume = 0;
		angle = 0;
		ret = false;
		break;
	}
	
	if (volume > 255)
	{
		volume = 255;
	}
	else if (volume < 0)
	{
		volume = 0;
	}

	Mix_SetPosition(channel, angle, volume);
	return ret;
}

void ModuleAudio::SilenceAllChannels(int id)
{
	Mix_HaltChannel(id);
}

void ModuleAudio::SilenceAll()
{
	SilenceAllChannels(-1);
	Mix_HaltMusic();
}

