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
	text = App->ui->GetAtlas();
	return true;
}

void j1UIelement::Draw(bool debug)
{
	if (hovering || (App->ui->focused && App->ui->focused->data == this))
	{
		SDL_SetTextureColorMod(text, 200, 200, 200);
		SDL_SetTextureAlphaMod(text, 255);
	}
	else
	{
		SDL_SetTextureColorMod(text, 255, 255, 255);
		SDL_SetTextureAlphaMod(text, 255);
	}

	if(enabled)
	App->render->Blit(text, Position.x, Position.y, &rect, 0.0f);

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
	SDL_Rect intersect = { Position.x, Position.y, rect.w, rect.h};

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


		App->ui->focused = nullptr;
	}

	return ret;	
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
		//TODO: Check boundaries if a bool is true

		PostoParent.x += currentPos.x - Position.x;
		PostoParent.y += currentPos.y - Position.y;
	}
}

void j1UIelement::KeepDistanceToParent(float dt)
{
	Position.x = parent->Position.x - PostoParent.x;
	Position.y = parent->Position.y - PostoParent.y;
}


