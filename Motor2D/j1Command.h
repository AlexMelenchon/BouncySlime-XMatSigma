#ifndef __j1COMMAND_H__
#define __j1COMMAND_H__

#include "p2Defs.h"
#include "p2SString.h"
#include "j1Module.h"

class j1Command
{
public:
	//--------INTERNAL CONTROL---------//
	//Constructor
	j1Command(const char* text, j1Module* callback, uint min_arg, uint max_arg, UIFunction func);

	//Destructor
	~j1Command();

	// Get the actual Command String
	p2SString GetCommandText();

	// Get the command description (used by the command list)
	p2SString GetDescription();

	//--------COMMAND STRUCTURE---------//

	//Pointer to the module that will recieve the command
	j1Module* callback = nullptr;

	//Min & Max arguments that a command admits
	uint min_arg = 0u;
	uint max_arg = 0u;

	//The function that the command does
	UIFunction function = UIFunction::FNC_NONE;

private:
	//--------COMMAND STRUCTURE---------//
	
	//String storage of the command description + it's actual command
	p2SString commandText;
	p2SString Description;

};


#endif // __j1COMMAND_H__