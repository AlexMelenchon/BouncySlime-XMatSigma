#ifndef __j1WINDOW_H__
#define __j1WINDOW_H__

#include "j1Module.h"

struct SDL_Window;
struct SDL_Surface;

class j1Window : public j1Module
{
public:
	//--------INTERNAL CONTROL---------//
	//Constructor
	j1Window();

	// Destructor
	virtual ~j1Window();

	// Called before render is available
	bool Awake(pugi::xml_node&);


	// Called at the end of the frame
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//--------WINDOW---------//
	// Changae title
	void SetTitle(const char* new_title);

	// Retrive window size
	void GetWindowSize(uint& width, uint& height) const;

	// Retrieve window scale
	uint GetScale() const;

	void ShowWindowTitle() const;

public:
	//--------WINDOW---------//
	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface;
	p2SString	title;
	uint		width = 0u;
	uint		height = 0u;
	uint		scale = 0u;

private:

};

#endif // __j1WINDOW_H__