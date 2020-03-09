#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Module.h"
#include "SDL_mixer\include\SDL_mixer.h"
#include "vector"

#define DEFAULT_MUSIC_FADE_TIME 2.0f
#define MAX_VOLUME 128

struct _Mix_Music;
struct Mix_Chunk;
enum class Direction {
	Front,
	FrontRight,
	Right,
	BackRight,
	Back,
	BackLeft,
	Left,
	FrontLeft
};
enum class Loudness {
	Quiet,
	Normal,
	Loud
};

class ModuleAudio : public Module
{
public:

	ModuleAudio();

	// Destructor
	virtual ~ModuleAudio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME, int volume = 180);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0, int channel = 1, Loudness loudness = Loudness::Loud, Direction direction = Direction::Front);

	//Configure Channel
	bool ConfigureChannel(unsigned int channel, int volume, float angle);
	bool ConfigureChannel(unsigned int channel,Loudness loudness, Direction direction);


private:
	P2SString fxFolder;

	_Mix_Music* music;

	std::vector<Mix_Chunk*>	fx;  //We need a Array of Numbers: sfx are stored as unsigned int
};

#endif // __AUDIO_H__