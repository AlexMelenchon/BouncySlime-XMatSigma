#include "j1UIelement.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1UIManager.h"
#include "j1Collision.h"
#include "j1EntityManager.h"

//Constructor
j1UIelement::j1UIelement()
{}

//Destructor
j1UIelement::~j1UIelement()
{}

// Called before the first frame
bool j1UIelement::Start()
{
	texture = App->ui->GetAtlas();
	return true;
}

//Draws the element into the screen
bool j1UIelement::Draw(bool debug)
{
	if (!enabled)
		return true;

	// If the element if focused or hovering (or its parent), we put on the texture a mask
	//If its focused (a.k.a pressed, we put on a greyer mask)
	if (this->IsFocused())
	{
		SDL_SetTextureColorMod(texture, 150, 150, 150);
	}
	else if (hovering || (parent && parent->hovering))
	{
		SDL_SetTextureColorMod(texture, 200, 200, 200);
	}
	//We return the texture to normal
	else
	{
		SDL_SetTextureColorMod(texture, 255, 255, 255);
	}

	//Blit the element into screen
	App->render->Blit(texture, Position.x, Position.y, &rect, 0.0f);

	//In debug mode, we also drag its rectangle
	if (debug)
	{
		App->render->DrawQuad({ Position.x, Position.y, rect.w, rect.h }, 0, 255, 255, 255, false, false);
	}

	return true;
}

//Checks if the mouse if hovering the element & if it should hover over it's siblings or parents
bool j1UIelement::OnHover()
{
	bool ret = false;

	//Check where is the mouse------
	SDL_Point mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);

	//Check if it intersects & if it CAN intersect
	SDL_Rect intersect = { Position.x, Position.y, rect.w, rect.h };
	if (SDL_PointInRect(&mouse, &intersect) && this->enabled && this->interact)
		ret = true;

	//Check in the hierarchy which element is on the mouse & if it is ready to drag
	if (ret)
	{
		for (p2List_item<j1UIelement*>* iterator = App->ui->GetElementFromList(this)->prev; iterator; iterator = iterator->prev)
		{
			if (iterator->data->parent != this->parent && iterator->data != this->parent)
				break;

			intersect = { iterator->data->Position.x, iterator->data->Position.y, iterator->data->rect.w, iterator->data->rect.h };

			if (SDL_PointInRect(&mouse, &intersect) && iterator->data->enabled && iterator->data->interact)
			{
				iterator->data->hovering = false;
				iterator->data->dragging = false;
			}

		}

	}


	return ret;
}

// Called each loop iteration
bool j1UIelement::PreUpdate()
{
	//Before the start of every frame, we check if the mouse is hovering on any object
	hovering = OnHover();

	return true;
}

// Called each loop iteration
bool j1UIelement::Update(float dt)
{
	if (!enabled)
		return true;

	//If the mouse is hovering above an object...
	if (hovering)
	{
		//We retrieve the focus accourding to logic
		if (App->ui->focused.lookAt && (App->ui->focused.state == focusState::ST_LOCKED && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) != KEY_REPEAT))
		{
			App->ui->focused.lookAt = nullptr;
			App->ui->focused.state = focusState::ST_FREE;
		}
		else if (App->ui->focused.lookAt && App->ui->focused.lookAt->data != this && App->ui->focused.state == focusState::ST_FREE && App->ui->focused.lookAt->data->type != ui_type::UI_INPUTBOX && App->ui->focused.lookAt->data->type != ui_type::UI_CONSOLE)
			App->ui->focused.lookAt = nullptr;

		//If we detect a click....
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			//We call for the appropiate function
			OnClick();

			// We start dragging if the object allows
			if (drag && !dragging)
			{
				dragging = true;

				//We check the point where the user clicked
				iPoint ClickedPoint = { 0,0 };
				App->input->GetMousePosition(ClickedPoint.x, ClickedPoint.y);
				MovePoint = { ClickedPoint.x - Position.x, ClickedPoint.y - Position.y };
			}

			//We start focusing on this object
			if (!App->ui->focused.lookAt || (App->ui->focused.lookAt && App->ui->focused.lookAt->data != this))
			{
				App->ui->focused.lookAt = App->ui->GetElementFromList(this);
			}
		}

	}

	//If we are focused in an object & we are draging....
	if (this->IsFocused() && App->ui->focused.state == focusState::ST_FREE)
	{

		if (dragging)
		{
			//Call for the appropiate function & move the object
			Move(dt);
			OnDrag();
		}

		// Check if we lose focus of the object
		if (DeFocus())
			return true;
	}

	//If the obect has a parent, we make sure it keeps it's distance
	if (parent)
	{
		KeepDistanceToParent();
	}

	// We call for the inherits custom update, if any
	InheritUpdate(dt);


	return true;

}

// Called before quitting
bool j1UIelement::CleanUp()
{
	bool ret = true;

	this->texture = nullptr;

	if (this->IsFocused())
		App->ui->focused.lookAt = nullptr;

	return ret;
}

//Calls for function when the object is pressed
void j1UIelement::OnClick()
{
	if (listener != nullptr && function != UIFunction::FNC_NONE)
	{
		this->listener->OnGui(UIEventType::EVENT_ONCLICK, this->function);
	}

}

//Calls for function when the object stops being pressed
bool j1UIelement::OnRelease()
{
	bool ret = false;

	if (listener != nullptr && function != UIFunction::FNC_NONE)
	{
		this->listener->OnGui(UIEventType::EVENT_UPCLICK, this->function);
		ret = true;
	}

	return ret;
}

//Calls for function when the object is dragged
void j1UIelement::OnDrag()
{
	if (listener != nullptr && function != UIFunction::FNC_NONE)
	{
		this->listener->OnGui(UIEventType::EVENT_DRAG, this->function);
	}
}

//Move the element
void j1UIelement::Move(float dt)
{
	//We check where the mouse is
	iPoint MousePos = { 0,0 };
	App->input->GetMousePosition(MousePos.x, MousePos.y);

	//We save the position before moving
	iPoint currentPos = Position;

	//Accourding to its axis of movement, we update the position,
	// according to our moving point, and where the mouse is placed on screen
	switch (axis)
	{
	case drag_axis::MOV_ALL:

		Position.x += ((MousePos.x - Position.x) - MovePoint.x);
		Position.y += ((MousePos.y - Position.y) - MovePoint.y);

		break;
	case drag_axis::MOV_X:

		Position.x += ((MousePos.x - Position.x) - MovePoint.x);

		break;
	case drag_axis::MOV_Y:

		Position.y += ((MousePos.y - Position.y) - MovePoint.y);

		break;
	}

	//If the object we just moved has a parent, we also update
	// it's relative position to it
	if (parent != nullptr)
	{
		PostoParent.x += currentPos.x - Position.x;
		PostoParent.y += currentPos.y - Position.y;
	}

}

//Make sure that, if the element has a parent, they maintain their distance to it
void j1UIelement::KeepDistanceToParent()
{
	Position.x = parent->Position.x - PostoParent.x;
	Position.y = parent->Position.y - PostoParent.y;
}

//Checks if the element has the focus or not
bool j1UIelement::IsFocused()
{
	return(App->ui->focused.lookAt && App->ui->focused.lookAt->data == this);
}

//Retrives focus when the conditions are met
bool j1UIelement::DeFocus()
{
	bool ret = false;

	//If we let go of the click, we lose focus (THE CONDITIONS MIGHT CHANGE IN CUSTOM DeFocus() IN THE INHERITS)
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_IDLE || App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
	{
		//We call for the function
		ret = App->ui->focused.lookAt->data->OnRelease();

		//Stop dragging & focusing
		if (App->ui->focused.lookAt != nullptr)
		{
			App->ui->focused.lookAt->data->dragging = false;
			App->ui->focused.lookAt = nullptr;
		}
	}

	return ret;
}

//Used to Update the elements position when it's not moving
void j1UIelement::Disable(bool to_disable)
{
	if (enabled != to_disable)
	{
		enabled = to_disable;

		for (p2List_item<j1UIelement*>* iterator = App->ui->GetElementFromList(this)->prev; iterator; iterator = iterator->prev)
		{
			if (iterator->data->parent == this)
				iterator->data->Disable(to_disable);
		}


		if (!enabled)
		{
			if (this->IsFocused())
				App->ui->focused.lookAt = nullptr;
		}
		else if (enabled)
		{
			App->ui->focused.lookAt = App->ui->GetElementFromList(App->scene->console);
		}

	}
}
