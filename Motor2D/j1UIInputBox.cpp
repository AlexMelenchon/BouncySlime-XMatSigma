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
	boxImage = App->ui->AddElement(ui_type::UI_IMAGE, this, this->Position, true, false, this->enabled, { 73, 992, 256, 64 }, this->listener, UIFunction::FNC_NONE);
	boxText = App->ui->AddElement(ui_type::UI_TEXT, this, this->Position, true, false, this->enabled, this->rect, this->listener, UIFunction::FNC_NONE, this->axis, txt);
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
	cursorTimer += dt;

	if (cursorTimer > 1.5f)
		cursorTimer = 0;

	return true;
}

bool j1UIInputBox::PostUpdate(bool debug)
{
	if (this->IsFocused())
	{
		App->input->WrittingState(true, boxImage->rect);

		int cursorPosition = App->input->GetTextInPos();
		if (cursorTimer > 0.75f)
		{
			App->render->DrawQuad({ boxText->Position.x + cursorPosition, boxText->Position.y,4, boxText->rect.h }, 200, 200, 200, 255, true, false);
		}
	}
	else
		App->input->WrittingState(false, boxImage->rect);

	return true;
}

bool j1UIInputBox::CleanUp()
{
	App->tex->UnLoad(texture);
	texture = nullptr;

	boxImage = nullptr;
	boxText = nullptr;
	return true;
}


void j1UIInputBox::DeFocus()
{
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_IDLE || App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
	{
		App->ui->focused.lookAt->data->OnRelease();
		App->ui->focused.lookAt->data->dragging = false;
	}

	if(App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && !this->hovering)
		App->ui->focused.lookAt = nullptr;
}
