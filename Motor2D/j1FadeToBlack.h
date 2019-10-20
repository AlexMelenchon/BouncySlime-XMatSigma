#ifndef __MODULEFADETOBLACK_H__
#define __MODULEFADETOBLACK_H__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

class j1FadeToBlack : public j1Module
{
public:
	j1FadeToBlack();
	~j1FadeToBlack();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PostUpdate();
	bool FadeToBlack(const char* mapNumber, float time = 2.0f);

private:

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

	j1Module* moduleOff;
	j1Module* moduleIn;

	
};

#endif //__MODULEFADETOBLACK_H__