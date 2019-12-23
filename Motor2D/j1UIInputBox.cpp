#include "j1UIInputBox.h"
#include "j1UIManager.h"

j1UIInputBox::j1UIInputBox()
{
}

j1UIInputBox::j1UIInputBox(const char* txt)
{
	this->type = ui_type::UI_INPUTBOX;
	App->ui->AddElement(ui_type::UI_IMAGE, this, this->Position, true, false, this->enabled, { 73, 406, 64, 64 }, this->listener, UIFunction::FNC_UNKOWNN);
	App->ui->AddElement(ui_type::UI_TEXT, this, this->Position, true, false, this->enabled, this->rect, this->listener, UIFunction::FNC_UNKOWNN, txt);
}

j1UIInputBox::~j1UIInputBox()
{
}

bool j1UIInputBox::Awake(pugi::xml_node&)
{
	return true;
}

bool j1UIInputBox::Start()
{
	return true;
}

bool j1UIInputBox::PostUpdate(bool debug)
{
	
	return true;
}

bool j1UIInputBox::CleanUp()
{
	return true;
}
