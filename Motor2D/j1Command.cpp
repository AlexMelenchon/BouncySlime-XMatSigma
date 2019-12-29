#include "j1Command.h"

//Constructor
j1Command::j1Command(const char* text, j1Module* callback, uint min_arg, uint max_arg, UIFunction func)
{
	//First we save the full description
	this->Description = text;

	//Then we save only the command word
	p2SString commandWord(Description);
	commandWord.Cut(commandWord.FindFirst(" "));

	this->commandText = commandWord;
	this->callback = callback;
	this->min_arg = min_arg;
	this->max_arg = max_arg;
	this->function = func;
}

//Destructor
j1Command::~j1Command()
{
	commandText.Clear();
	callback = nullptr;
}

// Get the actual Command String
p2SString j1Command::GetCommandText()
{
	return commandText;
}

// Get the command description
p2SString j1Command::GetDescription()
{
	return Description;
}