#include "j1Console.h"
#include "j1UIManager.h"
#include "j1Window.h"

j1Console::j1Console()
{}

j1Console::j1Console(char* txt, iPoint Position)
{
	this->type = ui_type::UI_CONSOLE;

	//Load the childs----
	image = App->ui->AddElement(ui_type::UI_IMAGE, this, Position, true, false, false, { NULL,0,0, (int)App->win->height / 4 }, this->listener, UIFunction::FNC_NONE, this->axis);
	inputBox = App->ui->AddElement(ui_type::UI_INPUTBOX, this, { Position.x, Position.y -(int)App->win->height /4 }, true, false, true, { 0,0,0,0 }, this->listener, UIFunction::FNC_NONE, drag_axis::MOV_NONE, txt);
}

j1Console::~j1Console()
{}

bool j1Console::InheritUpdate(float dt)
{
	bool ret = true;

	//If the console is focused, redirect to focus to the input box
	if (IsFocused())
	{
		App->ui->focused.lookAt = App->ui->GetElementFromList(inputBox);
	}
	//If the input box loses focus, disable the whole console
	else if (!inputBox->IsFocused())
		Disable(!enabled);


	return ret;
}

bool j1Console::PostUpdate(bool debug)
{
	bool ret = true;


	return ret;
}


bool j1Console::CleanUp()
{
	bool ret = true;

	inputBox = nullptr;
	image = nullptr;

	return ret;
}

bool j1Console::DeFocus()
{


	return false;
}
