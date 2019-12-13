#include "j1UIManager.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1UIButton.h"

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
	bool ret = false;

	p2List_item<j1UIelement*>* tmp = UIList.start;

		while (tmp != nullptr)
		{
			ret = tmp->data->PreUpdate();
			tmp = tmp->next;
		}

	return ret;
}

bool j1UIManager::Update(float dt)
{
	bool ret = false;

	p2List_item<j1UIelement*>* tmp = UIList.start;

	while (tmp != nullptr)
	{
		ret = tmp->data->Update(dt);
		tmp = tmp->next;
	}

	return ret;

}

// Called after all Updates
bool j1UIManager::PostUpdate()
{
	bool ret = false;

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

	return true;
}

// const getter for atlas
SDL_Texture* j1UIManager::GetAtlas() const
{
	return atlas;
}


j1UIelement* j1UIManager::AddElement(ui_type type, j1UIelement* parent, iPoint Position, bool interact, bool drag, bool enabled, SDL_Rect section, j1Module* listener)
{
	j1UIelement* tmp = nullptr;

	pugi::xml_node config;

	switch (type)
	{
	case ui_type::UI_BUTTON:
		tmp = new j1UIButton();
		break;
	}

	if (tmp)
	{
		tmp->parent = parent;
		if(parent == nullptr)
		tmp->Position = Position;
		else
		tmp->PostoParent = Position;
		tmp->interact = interact;
		tmp->drag = drag;
		tmp->enabled = enabled;
		tmp->rect = section;
		tmp->listener = listener;

		InitElement(UIList.add(tmp)->data, config);


	}


	return tmp;
}

void j1UIManager::InitElement(j1UIelement* element, pugi::xml_node config) 
{
	element->Awake(config);
	element->Start();
}