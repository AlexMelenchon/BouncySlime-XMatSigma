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

	void OptionsLoad(bool to_load);


private:

	bool optionsMenu = false;

};


#endif // __j1SCENE_H__