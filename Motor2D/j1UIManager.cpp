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
	return true;
}

bool j1UIManager::Update(float dt)
{
	return false;
}

// Called after all Updates
bool j1UIManager::PostUpdate()
{
	return true;
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


j1UIelement* j1UIManager::AddElement(ui_type type, j1UIelement* parent, iPoint gloalPos, iPoint localPos, bool interact, bool drag, bool enabled)
{
	j1UIelement* tmp;

	switch (type)
	{
	case ui_type::UI_BUTTON:
		tmp = new j1UIButton();
		break;


	}


	return nullptr;
}
