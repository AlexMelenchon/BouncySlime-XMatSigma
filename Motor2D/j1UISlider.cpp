#include "j1UISlider.h"
#include "j1UIManager.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Textures.h"

j1UISlider::j1UISlider()
{
	this->type = ui_type::UI_SLIDER;
	thumb = App->ui->AddElement(ui_type::UI_BUTTON, this, this->Position, true, true, this->enabled, { 73, 406, 64, 64 }, this->listener, UIFunction::FNC_UNKOWNN, this->axis);
	line = App->ui->AddElement(ui_type::UI_IMAGE, this, this->Position, false, false, this->enabled, {73,992,256,64}, this->listener, UIFunction::FNC_UNKOWNN, this->axis);
}

j1UISlider::j1UISlider(drag_axis axis)
{
	this->type = ui_type::UI_SLIDER;
	line = App->ui->AddElement(ui_type::UI_IMAGE, this, this->Position, false, false, this->enabled, { 73,992,256,64 }, this->listener, UIFunction::FNC_UNKOWNN, axis);
	thumb = App->ui->AddElement(ui_type::UI_BUTTON, this, this->Position, true, true, this->enabled, { 73, 406, 64, 64 }, this->listener, UIFunction::FNC_UNKOWNN, axis);	
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
	return true;
}

bool j1UISlider::PostUpdate(bool debug)
{
	Draw(debug);
	LOG("%i, %i", this->rect.w, this->rect.h);
	return true;
}

bool j1UISlider::CleanUp()
{
	return true;
}

