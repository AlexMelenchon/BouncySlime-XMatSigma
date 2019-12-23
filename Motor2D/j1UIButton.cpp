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

bool j1UIButton::Update(float dt)
{
	bool ret = true;


	if (hovering)
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
			OnClick();

		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
			OnRelease();

		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && !dragging && drag)
		{
			if(drag)
			dragging = true;

			iPoint ClickedPoint = { 0,0 };
			App->input->GetMousePosition(ClickedPoint.x, ClickedPoint.y);
			MovePoint = { ClickedPoint.x - Position.x, ClickedPoint.y - Position.y };

		}
	}


	if (dragging)
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_IDLE || App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		{
			dragging = false;
		}
		else
		{
			OnDrag();
			Move(dt);
		}
	}


	if (parent)
	{
		KeepDistanceToParent(dt);
	}


	return ret;
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

