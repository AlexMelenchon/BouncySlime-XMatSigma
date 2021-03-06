#ifndef __j1AUDIO_H__
#define __j1AUDIO_H__

#include "j1Module.h"
#include "SDL_mixer\include\SDL_mixer.h"
#include "p2List.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class j1Audio : public j1Module
{
public:

	//--------INTERNAL CONTROL---------//
	//Constructor
	j1Audio();

	// Destructor
	virtual ~j1Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);


	//--------AUDIO---------//
	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0, int channel = -1);

	//Unloads all Fx that are Loaded
	void UnLoadAllFx();

	//--------SAVE & LOAD---------//
//Called when loading a save
	bool Load(pugi::xml_node&);

	//Called to save the game
	bool Save(pugi::xml_node&) const;

	//Volume for sfx & music
	uint musicVolume = 0;
	uint fxVolume = 0;

private:
	//--------AUDIO---------//
	//Pointer to the level's track
	_Mix_Music*			music;
	//List to all sound effects
	p2List<Mix_Chunk*>	fx;
};

#endif // __j1AUDIO_H__