#include "j1ConsoleM.h"

//Constructor
j1ConsoleM::j1ConsoleM() : j1Module()
{
	name.create("console");
}

//Destructor
j1ConsoleM::~j1ConsoleM()
{}

// Called before render is available
bool j1ConsoleM::Awake(pugi::xml_node& consoleConfig)
{
	//Create commands related to console------
	//Log Command
	CreateCommand("log mystring (between 1-100 words)", this, 2, 100, UIFunction::FNC_LOG);
	//List Command
	CreateCommand("list", this, 1, 1, UIFunction::FNC_LIST);

	return true;
}

// Adds a Command into the list
j1Command* j1ConsoleM::CreateCommand(const char* command, j1Module* callback, uint min_arg, uint max_args, UIFunction function)
{
	j1Command* tmp = nullptr;

	//Check if all the variables are valid
	if (callback != nullptr && function != UIFunction::FNC_NONE)
	{
		tmp = new j1Command(command, callback, min_arg, max_args, function);
		this->commandList.add(tmp);
	}
	else
		LOG("Invalid command Introduced");

	return tmp;
}

// Called before quitting
bool j1ConsoleM::CleanUp()
{
	bool ret = true;

	//Iterate through all commands and delete them
	p2List_item<j1Command*>* item = commandList.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}

	commandList.clear();

	return ret;
}

// Manages Console Input
bool j1ConsoleM::ManageCommand(const char* command)
{
	bool ret = false;

	//Search for the first word of the command
	p2SString commandBuffer(command);
	commandBuffer.Cut(commandBuffer.FindFirst(" "));

	//Iterate though all commands if the list
	for (p2List_item<j1Command*>* iterator = commandList.start; iterator; iterator = iterator->next)
	{
		//If we happen to find one that meets the criteria
		if (iterator->data->GetCommandText() == commandBuffer)
		{
			j1Command* commandFound = iterator->data;
			commandBuffer = command;

			//We check if it has the number adequate of arguments
			int wordNum = commandBuffer.Find(" ") + 1;
			if (wordNum < commandFound->min_arg || wordNum > commandFound->max_arg)
				break;

			//If all the checks were positive, we execute the command
			switch (commandFound->function)
			{
			case UIFunction::FNC_LOG:
				commandBuffer.Cut(0, commandBuffer.FindFirst(" ") + 1);
				commandFound->callback->OnGui(UIEventType::EVENT_CONSOLE, commandFound->function, nullptr, commandBuffer.GetString());
				break;
			case UIFunction::FNC_GODMODE:
				commandFound->callback->OnGui(UIEventType::EVENT_CONSOLE, commandFound->function);
				break;

			case UIFunction::FNC_QUIT:
				commandFound->callback->OnGui(UIEventType::EVENT_CONSOLE, commandFound->function);
				break;

			case UIFunction::FNC_LIST:
				commandFound->callback->OnGui(UIEventType::EVENT_CONSOLE, commandFound->function);
				break;

			case UIFunction::FNC_LOADMAP:
				commandBuffer.Cut(0, commandBuffer.FindFirst(" ") + 1);
				commandFound->callback->OnGui(UIEventType::EVENT_CONSOLE, commandFound->function, nullptr, commandBuffer.GetString());
				break;

			case UIFunction::FNC_FPS:
				commandBuffer.Cut(0, commandBuffer.FindFirst(" ") + 1);
				commandFound->callback->OnGui(UIEventType::EVENT_CONSOLE, commandFound->function, nullptr, commandBuffer.GetString());
				break;

			}

			ret = true;
			break;
		}

	}


	return ret;
}

//Manages the UI inputs of this module
void j1ConsoleM::OnGui(UIEventType type, UIFunction func, j1UIelement* userPointer, const char* bufferText)
{
	switch (type)
	{
	case UIEventType::EVENT_CONSOLE:

		switch (func)
		{
		case UIFunction::FNC_LOG:
			LOG("LOG: %s", bufferText);
			break;

		case UIFunction::FNC_LIST:
		{
			int CommandNum = 1;
			for (p2List_item<j1Command*>* iterator = commandList.start; iterator; iterator = iterator->next)
			{
				LOG("%d: %s", CommandNum, iterator->data->GetDescription().GetString());
				CommandNum++;
			}
		}
		break;
		}
	}
}