#include "j1UIButton.h"



j1UIButton::j1UIButton()
{

}

bool j1UIButton::Awake(pugi::xml_node&)
{
	this->type = ui_type::UI_BUTTON;

	return true;
}

bool j1UIButton::Start()
{
	return true;
}

bool j1UIButton::PreUpdate()
{
	//detect if mouse is on the element
	//detect if the element ios clicked
	//make logic when mouse click is on key_up
	bool aux = OnHover();


	return true;
}

bool j1UIButton::Update(float dt)
{
	return true;
}

bool j1UIButton::PostUpdate()
{
	return true;
}

bool j1UIButton::CleanUp()
{
	return true;
}

void j1UIButton::OnClick()
{

}

void j1UIButton::OnRelease()
{

}
