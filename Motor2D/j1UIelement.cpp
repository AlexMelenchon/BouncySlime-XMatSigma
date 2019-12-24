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

j1UIelement::j1UIelement()
{}

j1UIelement::~j1UIelement()
{}

bool j1UIelement::Start()
{
	texture = App->ui->GetAtlas();
	return true;
}

void j1UIelement::Draw(bool debug)
{
	if (App->ui->focused.lookAt && App->ui->focused.lookAt->data == this)
	{
		SDL_SetTextureColorMod(texture, 150, 150, 150);
	}
	else if (hovering)
	{
		SDL_SetTextureColorMod(texture, 200, 200, 200);
	}
	else
	{
		SDL_SetTextureColorMod(texture, 255, 255, 255);
	}


	App->render->Blit(texture, Position.x, Position.y, &rect, 0.0f);

	if (debug)
	{
		App->render->DrawQuad({ Position.x, Position.y, rect.w, rect.h }, 0, 255, 255, 255, false, false);
	}
}

bool j1UIelement::OnHover()
{
	bool ret = false;
	SDL_Point mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);
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

bool j1UIelement::PreUpdate()
{
	hovering = OnHover();

	return true;
}


bool j1UIelement::Update(float dt)
{
	if (hovering)
	{
		if (App->ui->focused.lookAt && (App->ui->focused.state == focusState::ST_LOCKED && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) != KEY_REPEAT))
		{
			App->ui->focused.lookAt = nullptr;
			App->ui->focused.state = focusState::ST_FREE;
		}
		else if (App->ui->focused.lookAt && App->ui->focused.lookAt->data != this && App->ui->focused.state == focusState::ST_FREE)
			App->ui->focused.lookAt = nullptr;

		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			OnClick();

			if (drag && !dragging)
			{
				dragging = true;

				iPoint ClickedPoint = { 0,0 };
				App->input->GetMousePosition(ClickedPoint.x, ClickedPoint.y);
				MovePoint = { ClickedPoint.x - Position.x, ClickedPoint.y - Position.y };
			}

			if (!App->ui->focused.lookAt || (App->ui->focused.lookAt && App->ui->focused.lookAt->data != this))
			{
				App->ui->focused.lookAt = App->ui->GetElementFromList(this);
			}
		}

	}

	if (App->ui->focused.lookAt && App->ui->focused.lookAt->data == this && App->ui->focused.state == focusState::ST_FREE)
	{
		if (dragging)
		{
			OnDrag();
			Move(dt);
		}

		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_IDLE || App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		{
			App->ui->focused.lookAt->data->OnRelease();
			App->ui->focused.lookAt->data->dragging = false;
			//App->ui->focused.lookAt = nullptr;
		}

	}


	if (parent)
	{
		KeepDistanceToParent(dt);
	}

	InheritUpdate(dt);


	return true;

}

void j1UIelement::OnClick()
{
	if (listener != nullptr)
	{
		this->listener->OnGui(UIEventType::EVENT_ONCLICK, this->function);
	}

}

void j1UIelement::OnRelease()
{
	if (listener != nullptr)
	{
		this->listener->OnGui(UIEventType::EVENT_UPCLICK, this->function);
	}
}

void j1UIelement::OnDrag()
{
	if (listener != nullptr)
	{
		this->listener->OnGui(UIEventType::EVENT_DRAG, this->function);
	}
}

void j1UIelement::Move(float dt)
{
	iPoint MousePos = { 0,0 };
	App->input->GetMousePosition(MousePos.x, MousePos.y);

	iPoint currentPos = Position;
	Position.x += ((MousePos.x - Position.x) - MovePoint.x);
	Position.y += ((MousePos.y - Position.y) - MovePoint.y);


	if (parent != nullptr)
	{
		//TODO: Check boundaries on the slider after this func!

		PostoParent.x += currentPos.x - Position.x;
		PostoParent.y += currentPos.y - Position.y;
	}

}

void j1UIelement::KeepDistanceToParent(float dt)
{
	Position.x = parent->Position.x - PostoParent.x;
	Position.y = parent->Position.y - PostoParent.y;
}


