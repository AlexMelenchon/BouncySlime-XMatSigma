#include "j1UIManager.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1UIButton.h"
#include "j1UIText.h"
#include "j1UIImage.h"
#include "j1UIInputBox.h"
#include "j1UISlider.h"
#include "j1Console.h"

j1UIManager::j1UIManager() : j1Module()
{
	name.create("gui");
}

// Destructor
j1UIManager::~j1UIManager()
{}

// Called before render is available
bool j1UIManager::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1UIManager::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool j1UIManager::PreUpdate()
{
	bool ret = true;

	p2List_item<j1UIelement*>* tmp = UIList.start;

	while (tmp != nullptr)
	{
		ret = tmp->data->PreUpdate();
		tmp = tmp->next;
	}

	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
		ChangeFocus();


	return ret;
}

bool j1UIManager::Update(float dt)
{
	bool ret = true;

	p2List_item<j1UIelement*>* tmp = UIList.start;

	while (tmp != nullptr)
	{
		if (tmp != nullptr && tmp->data && tmp->data->type != ui_type::UI_NONE)
			ret = tmp->data->Update(dt);

		tmp = tmp->next;
	}

	return ret;

}

// Called after all Updates
bool j1UIManager::PostUpdate()
{
	bool ret = true;

	p2List_item<j1UIelement*>* tmp = UIList.start;

	while (tmp != nullptr)
	{
		ret = tmp->data->PostUpdate(debug);
		tmp = tmp->next;
	}

	return ret;
}

// Called before quitting
bool j1UIManager::CleanUp()
{
	LOG("Freeing GUI");

	//Iterate though all the remaining entities cleanUp
	DeleteAllElements();


	bool ret = App->tex->UnLoad(atlas);
	atlas = nullptr;

	return ret;
}

// const getter for atlas
SDL_Texture* j1UIManager::GetAtlas() const
{
	return atlas;
}


j1UIelement* j1UIManager::AddElement(ui_type type, j1UIelement* parent, iPoint Position, bool interact, bool drag, bool enabled, SDL_Rect section, j1Module* listener, UIFunction func, drag_axis axis, char* text)
{
	j1UIelement* tmp = nullptr;

	pugi::xml_node config;

	switch (type)
	{
	case ui_type::UI_BUTTON:
		if (strlen(text) > 1)
			tmp = new j1UIButton(text);

		else
			tmp = new j1UIButton();
		break;

	case ui_type::UI_TEXT:

		if (strlen(text) > 1)
		{
			tmp = new j1UIText(text);
			App->fonts->CalcSize(text, section.w, section.h);
		}
		break;

	case ui_type::UI_IMAGE:

		tmp = new j1UIImage();
		break;

	case ui_type::UI_INPUTBOX:

		tmp = new j1UIInputBox(text);
		App->fonts->CalcSize(text, section.w, section.h);

		break;
	case ui_type::UI_SLIDER:
		tmp = new j1UISlider(axis, Position);
		break;

	case ui_type::UI_CONSOLE:
		tmp = new j1Console(text, Position);
		break;
	}

	if (tmp)
	{
		tmp->parent = parent;


		if (parent == nullptr)
			tmp->Position = Position;
		else
		{
			tmp->PostoParent = Position;
			tmp->Position.x = parent->Position.x + Position.x;
			tmp->Position.y = parent->Position.y + Position.y;

		}

		tmp->interact = interact;
		tmp->drag = drag;
		tmp->enabled = enabled;
		tmp->rect = section;
		tmp->listener = listener;
		tmp->function = func;
		tmp->axis = axis;

		InitElement(UIList.add(tmp)->data, config);
	}

	return tmp;
}


void j1UIManager::InitElement(j1UIelement* element, pugi::xml_node config)
{
	element->Start();
}


p2List_item<j1UIelement*>* j1UIManager::GetElementFromList(j1UIelement* toSearch)
{
	for (p2List_item<j1UIelement*>* iterator = UIList.start; iterator; iterator = iterator->next)
	{
		if (iterator->data == toSearch)
			return iterator;
	}


	return nullptr;
}

void j1UIManager::ChangeFocus()
{
	if (focused.lookAt && focused.lookAt->next)
		focused.lookAt = focused.lookAt->next;
	else if (!focused.lookAt)
	{
		focused.lookAt = UIList.start;
		focused.state = focusState::ST_LOCKED;
	}
	else
	{
		focused.lookAt = nullptr;
		focused.state = focusState::ST_FREE;
	}
}

void j1UIManager::DeleteAllElements()
{
	//Iterate though all the elements and delete them
	p2List_item<j1UIelement*>* tmp = UIList.start;
	while (tmp != nullptr)
	{
		tmp->data->CleanUp();
		RELEASE(tmp->data);
		UIList.del(tmp);
		tmp = tmp->next;
	}
	UIList.clear();
}

void j1UIManager::DeleteElement(p2List_item<j1UIelement*>* element)
{
	p2List_item<j1UIelement*>* item = UIList.start;
	while (item != nullptr)
	{
		if (item->data->parent == element->data)
		{
			item->data->CleanUp();
			RELEASE(item->data);
			UIList.del(item);
		}
		item = item->next;

	}


	element->data->CleanUp();
	RELEASE(element->data);
	UIList.del(element);
}

void j1UIManager::ToDeleteElement()
{
	p2List_item<j1UIelement*>* item = UIList.start;
	while (item != nullptr)
	{
		if (item->data->to_delete)
		{
			App->ui->DeleteElement(item);
			item = UIList.start;
		}

		item = item->next;

	}


}


