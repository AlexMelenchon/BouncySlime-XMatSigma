#ifndef __j1MAINMENU_H__
#define __j1MAINMENU_H__

#include "j1Module.h"

class j1MainMenu : public j1Module
{
public:
	//--------INTERNAL CONTROL---------//
	//Constructor
	j1MainMenu();

	// Destructor
	virtual ~j1MainMenu();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//--------UI---------//
	//Manages the UI inputs of this module
	void OnGui(UIEventType type, UIFunction func, j1UIelement* userPointer = nullptr);

	//Loads the options sub-menu
	void MenusLoad(UIFunction func);


private:

	//--------UI LOGIC---------//
	
	//Used to exit the game
	bool exiting = true;

	//Used to load/unload the options submenu
	UIFunction lastcall;

	//"Ghost" UI used for position reference to the sub-menus
	j1UIelement* parent = nullptr;

	p2SString menuMusic;

};


#endif // __j1SCENE_H__
