#include "j1Command.h"

j1Command::j1Command(const char* text, j1Module* callback, uint min_arg, uint max_arg, UIFunction func)
{
	this->commandText = text;
	this->callback = callback;
	this->min_arg = min_arg;
	this->max_arg = max_arg;
	this->function = func;
}

j1Command::~j1Command()
{
	commandText.Clear();
	callback = nullptr;
}

p2SString j1Command::GetCommandText()
{
	return commandText;
}