#ifndef __MODULEFADETOBLACK_H__
#define __MODULEFADETOBLACK_H__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

class j1FadeToBlack : public j1Module
{
public:
	//--------INTERNAL CONTROL---------//
	//Constructor
	j1FadeToBlack();
	//Destructor
	~j1FadeToBlack();
	// Called before render is avaliable
	bool Awake(pugi::xml_node& config);
	// Called before the first frame iteration
	bool Start();
	// Called each loop iteration
	bool PostUpdate();

	//--------FADE---------//
	//Fades from one map to another
	bool FadeToBlackMap(const char* mapNumber, int id, float time = 2.0f);

	//Fades from modules (menu to game & viceversa)
	bool FadeToBlackMod(j1Module* SceneIn, j1Module* SceneOut, float time, bool load = false);


private:
	//--------FADE---------//
	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen;
	const char* mapToLoad = nullptr;

	//Indicates the modules to load & unload, if any
	j1Module* moduleOff = nullptr;
	j1Module* ModuleOn = nullptr;

	//Flag to order the fadeToBlack to call for the game load
	bool load = false;

	
};

#endif //__MODULEFADETOBLACK_H__