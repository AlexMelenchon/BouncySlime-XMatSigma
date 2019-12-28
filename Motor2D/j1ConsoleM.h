#ifndef __j1CONSOLE_H__
#define __j1CONSOLE_H__

#include "j1Module.h"
#include "j1Command.h"

class j1ConsoleM : public j1Module
{

public:
	//--------INTERNAL CONTROL---------//

	//Constructor
	j1ConsoleM();

	// Destructor
	virtual ~j1ConsoleM();

	// Called before render is available
	bool Awake(pugi::xml_node& consoleConfig);

	// Called before quitting
	bool CleanUp();

	// Manages Console Input
	bool ManageCommand(const char* command);

	//--------UI----------------------//
	//Manages the UI inputs of this module
	void OnGui(UIEventType type, UIFunction func, j1UIelement* userPointer = nullptr, const char* bufferText = "");

	//--------CONSOLE RELATED---------//
	// Adds a Command into the list
	j1Command* CreateCommand(const char* command, j1Module* callback, uint min_arg, uint max_args, UIFunction function);

private:

	//--------CONSOLE RELATED---------//
	//Contains a list of all commands
	p2List<j1Command*> commandList;
};

#endif // __j1CONSOLE_H__