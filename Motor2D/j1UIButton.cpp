#include "j1UIButton.h"
#include "j1Scene.h"
#include "j1Input.h"


j1UIButton::j1UIButton()
{
	this->type = ui_type::UI_BUTTON;
}

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

	if (hovering && (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN))
		OnClick();


	return true;
}

bool j1UIButton::Update(float dt)
{
	return true;
}

bool j1UIButton::PostUpdate()
{

	Draw();

	return true;
}

bool j1UIButton::CleanUp()
{
	return true;
}

void j1UIButton::OnClick()
{
	App->pause = !App->pause;
}

void j1UIButton::OnRelease()
{

}
