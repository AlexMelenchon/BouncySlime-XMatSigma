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

	line = App->ui->AddElement(ui_type::UI_IMAGE, this, { 0,0 }, false, false, true, { 73,992,256,64 }, this->listener, UIFunction::FNC_UNKOWNN, this->axis);
	thumb = App->ui->AddElement(ui_type::UI_BUTTON, this, { 0,0 }, true, true, true, { 73, 406, 64, 64 }, this->listener, UIFunction::FNC_UNKOWNN, this->axis);

}

j1UISlider::~j1UISlider()
{}

bool j1UISlider::Awake(pugi::xml_node&)
{
	return true;
}

bool j1UISlider::Start()
{

	return true;
}

bool j1UISlider::InheritUpdate(float dt)
{
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
	return true;
}

void j1UISlider::CheckLimits()
{
	if (thumb->PostoParent.x > 0)
	{
		thumb->PostoParent.x = 0;

		thumb->Position.x = thumb->parent->Position.x + thumb->PostoParent.x;

	}
	else if (thumb->PostoParent.x < (-line->rect.w + thumb->rect.w))
	{
		thumb->PostoParent.x = -line->rect.w + thumb->rect.w;
		thumb->Position.x = thumb->parent->Position.x - thumb->PostoParent.x;
	}


}