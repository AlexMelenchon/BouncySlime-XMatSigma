#include "j1UISlider.h"
#include "j1UIManager.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Textures.h"

j1UISlider::j1UISlider()
{
	this->type = ui_type::UI_SLIDER;
}

j1UISlider::j1UISlider(drag_axis axis)
{
	this->type = ui_type::UI_SLIDER;
	this->axis = axis;

	line = App->ui->AddElement(ui_type::UI_IMAGE, this, { 0,0 }, false, false, true, { 73,992,256,64 }, this->listener, UIFunction::FNC_NONE, this->axis);
	thumb = App->ui->AddElement(ui_type::UI_BUTTON, this, { 0,0 }, true, true, true, { 73, 406, 64, 64 }, this->listener, UIFunction::FNC_NONE, this->axis);
}

j1UISlider::~j1UISlider()
{}

bool j1UISlider::Awake(pugi::xml_node&)
{
	return true;
}

bool j1UISlider::Start()
{
	line->listener = this->listener;
	thumb->listener = this->listener;

	line->function = this->function;
	thumb->function = this->function;
	return true;
}

bool j1UISlider::InheritUpdate(float dt)
{
	//The childs follow the father's enabled
	thumb->enabled = this->enabled;
	line, enabled = this->enabled;

	//Check Bounadries
	CheckLimits();

	return true;
}

bool j1UISlider::PostUpdate(bool debug)
{
	Draw(debug);
	return true;
}

bool j1UISlider::CleanUp()
{	
	texture = nullptr;

	thumb = nullptr;
	line = nullptr;

	return true;
}

void j1UISlider::CheckLimits()
{
	// Left
	if (thumb->PostoParent.x > 0)
	{
		thumb->PostoParent.x = 0;
		thumb->Position.x = thumb->parent->Position.x + thumb->PostoParent.x;

	}
	//Right
	else if (thumb->PostoParent.x < (-line->rect.w + thumb->rect.w))
	{
		thumb->PostoParent.x = -line->rect.w + thumb->rect.w;
		thumb->Position.x = thumb->parent->Position.x - thumb->PostoParent.x;
	}
}

int j1UISlider::GetAudioValue()
{
	return round((-(float(-thumb->PostoParent.x) / ((float(-line->rect.w)) + float(thumb->rect.w))) * 128));
}
