#include "j1Console.h"
#include "j1UIManager.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Scene.h"

//Constructors----
j1Console::j1Console()
{
	this->type = ui_type::UI_CONSOLE;
}

j1Console::j1Console(char* txt, iPoint Position)
{
	this->type = ui_type::UI_CONSOLE;
	
	//Set the pointer to this, used to manipulate the console from the log.cpp
	App->scene->console = this;

	//Load the childs where the commands will we shown----
	image = App->ui->AddElement(ui_type::UI_IMAGE, this, Position, true, false, false, { NULL,0,0, (int)App->win->height / 4 }, this->listener, UIFunction::FNC_NONE, this->axis);
	Ctext = App->ui->AddElement(ui_type::UI_TEXT, this, { Position.x, Position.y }, true, false, true, this->rect, this->listener, UIFunction::FNC_NONE, this->axis, "Please introduce your commands below!");

	//Load the input box-----
	inputBox = App->ui->AddElement(ui_type::UI_INPUTBOX, this, { Position.x, Position.y - (int)(App->win->height * 0.25) }, true, false, true, { 0,0,0,0 }, this->listener, UIFunction::FNC_NONE, drag_axis::MOV_NONE, txt);
}

//Destructors
j1Console::~j1Console()
{}

//Called every frame after the element update
bool j1Console::InheritUpdate(float dt)
{
	bool ret = true;

	//If the console is focused, redirect to focus to the input box
	if (IsFocused())
	{
		App->ui->focused.lookAt = App->ui->GetElementFromList(inputBox);
	}

	return ret;
}

// Called before all Updates
bool j1Console::PostUpdate(bool debug)
{
	bool ret = true;


	return ret;
}

// Called before quitting
bool j1Console::CleanUp()
{
	bool ret = true;

	inputBox = nullptr;
	image = nullptr;
	App->scene->console = nullptr;
	consoleBuffer.Clear();

	return ret;
}

//Retrives focus when the conditions are met
bool j1Console::DeFocus()
{
	return false;
}

//Updates the text to show
void j1Console::UpdateText(const char* newLogEntry)
{
	// First of all, we paste the new log into the Screen
	//We add flags at the start & end, the actual string & an "\n"
	consoleBuffer += "> ";
	consoleBuffer += newLogEntry;
	consoleBuffer += ";";
	consoleBuffer += "\n";

	// We recalculate the the texture
	App->tex->UnLoad(Ctext->texture);
	Ctext->texture = App->fonts->Print(consoleBuffer.GetString());
	App->fonts->CalcSize(consoleBuffer.GetString(), Ctext->rect.w, Ctext->rect.h);

	//If the console text exceeds
	if (Ctext->rect.h > (int)App->win->height / 4)
	{
		consoleBuffer.Cut(consoleBuffer.FindFirst(">"), consoleBuffer.FindFirst(";")+2);
	}
}

//Called when the console is disabled
void j1Console::Disable()
{
	if (this->IsFocused())
		App->ui->focused.lookAt = nullptr;

	//If the input box loses focus, disable the whole console
	SetToDisable(false);

}

