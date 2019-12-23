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

bool j1UIButton::PreUpdate()
{
	//detect if mouse is on the element
	//detect if the element ios clicked
	//make logic when mouse click is on key_up
	hovering = OnHover();

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

