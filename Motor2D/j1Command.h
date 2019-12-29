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

	// Get text from String
	p2SString GetCommandText();
	p2SString GetDescription();

	j1Module* callback = nullptr;
	uint min_arg = 0u;
	uint max_arg = 0u;
	UIFunction function = UIFunction::FNC_NONE;

private:
	p2SString commandText;
	p2SString Description;

};


#endif // __j1COMMAND_H__