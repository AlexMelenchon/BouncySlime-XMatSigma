#include "j1UIButton.h"
#include "j1Scene.h"
#include "j1Input.h"


j1UIButton::j1UIButton()
{
	this->type = ui_type::UI_BUTTON;
}

j1UIButton::~j1UIButton()
{}

bool j1UIButton::Awake(pugi::xml_node&)
{

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

bool j1UIButton::CleanUp()
{
	return true;
}

