#include "j1UIInputBox.h"
#include "j1UIManager.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Textures.h"

j1UIInputBox::j1UIInputBox()
{}

j1UIInputBox::j1UIInputBox(char* txt)
{
	this->type = ui_type::UI_INPUTBOX;
	boxImage = App->ui->AddElement(ui_type::UI_IMAGE, this, this->Position, true, false, this->enabled, { 73, 406, 64, 64 }, this->listener, UIFunction::FNC_UNKOWNN);
	boxText = App->ui->AddElement(ui_type::UI_TEXT, this, this->Position, true, false, this->enabled, this->rect, this->listener, UIFunction::FNC_UNKOWNN, txt);
}

j1UIInputBox::~j1UIInputBox()
{}

bool j1UIInputBox::Awake(pugi::xml_node&)
{


	return true;
}

bool j1UIInputBox::Start()
{
	return true;
}

bool j1UIInputBox::InheritUpdate(float dt)
{
	return true;
}

bool j1UIInputBox::PostUpdate(bool debug)
{
	if (App->ui->focused.lookAt && App->ui->focused.lookAt->data == this)
	{
		App->input->WrittingState(true, boxImage->rect);
		App->render->DrawQuad({ boxText->Position.x + boxText->rect.w , boxText->Position.y,5, boxText->rect.h }, 255, 255, 255, 255, true, false);
	}
	else
		App->input->WrittingState(false, boxImage->rect);


	return true;
}

bool j1UIInputBox::CleanUp()
{
	return true;
}
