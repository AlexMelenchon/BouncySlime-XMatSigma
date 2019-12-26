// ----------------------------------------------------
// j1Module.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __j1MODULE_H__
#define __j1MODULE_H__

//General includes that we will use all across the modules
#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"
#include "p2Point.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_rect.h"
#include "SDL/include/SDL.h"
#include "p2List.h"
#include "p2Log.h"
#include"p2animation.h"
#include "p2DynArray.h"
#include "Brofiler/include/Brofiler.h"

class j1App;

struct SDL_Texture; // Struct for texture, to be able to call it from any module
struct SDL_Rect; // Struct for rect, to be able to call it from any module
class Animation; //Class for animations
class j1UIelement;


struct Collider;
enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,

	COLLIDER_WALL,
	COLLIDER_PLAYER,
	COLLIDER_START,
	COLLIDER_DEATH,
	COLLIDER_WIN,
	COLLIDER_GOD,
	COLLIDER_ENEMY,
	COLLIDER_SHURIKEN,

	COLLIDER_MAX
};

enum class UIEventType
{
	EVENT_UNKOWNN = -1,

	EVENT_ONCLICK,
	EVENT_UPCLICK,
	EVENT_DRAG
};

enum class UIFunction
{
	FNC_NONE = -1,

	//Main Menu
	FNC_STARTGAME,
	FNC_CONTINUEGAME,
	FNC_OPTIONS,
	FNC_CREDITS,
	FNC_EXIT,

	//In-Game
	FNC_PAUSE,
	FNC_EXITGAME,
	FNC_SAVE,

	//Both
	FNC_CHANGE_VMUSIC,
	FNC_CHANGE_VFX,




};

#define VEL_TO_WORLD 20


class j1Module
{
public:
	//--------INTERNAL CONTROL---------//
	//Constructor if active
	j1Module() : active(false)
	{}

	//Called before everything else
	void Init()
	{
		active = true;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	// Loads the game
	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	//Saves the game
	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}

	// Manages the UI inputs for each module
	virtual void OnGui(UIEventType type, UIFunction func, j1UIelement* userPointer = nullptr) {};

	//Distributes collisions
	virtual void OnCollision(Collider*, Collider*) {}

	//Checks if the module is disabled, and doesn't iterate it if dnot active
	bool IsEnabled() const { return active; }

	//Active a module
	void Enable()
	{
		if (active == false)
		{
			active = true;
			Start();
		}
	}

	//Disables a module
	void Disable()
	{
		if (active == true)
		{
			active = false;
			CleanUp();
		}
	}


public:
	//--------INTERNAL CONTROL---------//
	//A name for each module
	p2SString	name;
	//Sets if a module is active (if not, it doesn't iterate with it)
	bool		active;

};

#endif // __j1MODULE_H__