#ifndef __j1FONTS_H__
#define __j1FONTS_H__

#include "j1Module.h"
#include "SDL\include\SDL_pixels.h"
#include "SDL_TTF\include\SDL_ttf.h"

#define DEFAULT_FONT "textures/fonts/open_sans/OpenSans-Regular.ttf"
#define DEFAULT_FONT_SIZE 12

struct SDL_Texture;
struct _TTF_Font;

class j1Fonts : public j1Module
{
public:
	//--------INTERNAL CONTROL---------//

	//Constructor
	j1Fonts();

	// Destructor
	virtual ~j1Fonts();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Load Font
	_TTF_Font* const Load(const char* path, int size = 12);

	// Create a surface from text
	SDL_Texture* Print(const char* text, SDL_Color color = { 255, 255, 255, 255 }, _TTF_Font* font = nullptr);

	//Calculates the size of a text
	bool CalcSize(const char* text, int& width, int& height, _TTF_Font* font = NULL) const;

public:
	//--------FONTS---------//
	p2List<_TTF_Font*>	fonts;
	_TTF_Font* default_font;
};


#endif // __j1FONTS_H__