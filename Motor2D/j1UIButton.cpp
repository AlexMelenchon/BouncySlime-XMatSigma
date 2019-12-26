#include "j1UIButton.h"
#include "j1Scene.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1UIManager.h"


j1UIButton::j1UIButton()
{
	this->type = ui_type::UI_BUTTON;
}

j1UIButton::j1UIButton(char* text)
{
	this->type = ui_type::UI_BUTTON;

	this->text = text;
}

j1UIButton::~j1UIButton()
{}

bool j1UIButton::Awake(pugi::xml_node& awake)
{
	if(text && strlen(text) > 1)
	App->ui->AddElement(ui_type::UI_TEXT, this, {- this->rect.w / 2,-this->rect.h / 2 }, false, false, true,{ 0,0,0,0 }, nullptr, UIFunction::FNC_NONE, drag_axis::MOV_NONE, text);
	return true;
}

bool j1UIButton::InheritUpdate(float dt)
{
	return true;
}

bool j1UIButton::PostUpdate(bool debug)
{

	Draw(debug);

	return true;
}

void j1UIButton::OnDrag()
{
	if (listener != nullptr && parent != nullptr)
	{
		this->listener->OnGui(UIEventType::EVENT_DRAG, this->parent->function, this->parent);
	}
}

