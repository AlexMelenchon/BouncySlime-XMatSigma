#include "j1UIButton.h"
#include "j1Scene.h"
#include "j1Input.h"
#include "j1UIText.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Textures.h"

j1UIText::j1UIText()
{
	this->type = ui_type::UI_TEXT;
}
j1UIText::j1UIText(char* txt)
{
	this->type = ui_type::UI_TEXT;
	this->texture =	App->fonts->Print(txt);

}

j1UIText::~j1UIText()
{}

bool j1UIText::Awake(pugi::xml_node&)
{
	return true;
}

bool j1UIText::Start()
{
	return true;
}


bool j1UIText::PostUpdate(bool debug)
{
	App->tex->UnLoad(texture);
	texture = App->fonts->Print(App->input->GetText());

	App->fonts->CalcSize(App->input->GetText(), rect.w, rect.h);

	Draw(debug);
	return true;
}

bool j1UIText::CleanUp()
{
	return true;
}
