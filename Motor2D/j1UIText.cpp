#include "j1UIButton.h"
#include "j1Scene.h"
#include "j1Input.h"
#include "j1UIText.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Textures.h"


//Constructors----
j1UIText::j1UIText()
{
	this->type = ui_type::UI_TEXT;
}
j1UIText::j1UIText(char* txt)
{
	this->type = ui_type::UI_TEXT;
	this->texture = App->fonts->Print(txt);

}

//Destructors
j1UIText::~j1UIText()
{}

// Called before the first frame
bool j1UIText::Start()
{
	if (parent && parent->type == ui_type::UI_BUTTON)
	{
		this->PostoParent.x = -parent->rect.w / 2 + this->rect.w /2;
		this->PostoParent.y = -parent->rect.h / 2 + this->rect.h / 2;
	}
	return true;
}

// Called before all Updates
bool j1UIText::PostUpdate(bool debug)
{
	//If the button is part of an input box, we update the texture is the text changes
	if (parent && parent->type == ui_type::UI_INPUTBOX && parent->IsFocused())
	{
		App->tex->UnLoad(texture);
		texture = App->fonts->Print(App->input->GetText());

		App->fonts->CalcSize(App->input->GetText(), rect.w, rect.h);
	}

	Draw(debug);
	return true;
}

// Called before quitting
bool j1UIText::CleanUp()
{
	//Unload the texture
	App->tex->UnLoad(texture);
	texture = nullptr;
	
	//Deletes loaded text, if any
	delete text;
	text = nullptr;

	return true;
}
