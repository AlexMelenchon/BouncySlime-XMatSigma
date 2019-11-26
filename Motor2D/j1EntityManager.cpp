#include "j1EntityManager.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1LandEnemy.h"
#include "j1Map.h"
#include "j1Collision.h"

//Constructor
j1EntityManager::j1EntityManager()
{
	name.create("entities");
}

//Destructor
j1EntityManager::~j1EntityManager()
{
}

// Called before render is available
bool j1EntityManager::Awake(pugi::xml_node& config)
{
	bool ret = false;

	p2List_item<j1Entity*>* tmp = EntityList.start;
	if (tmp == nullptr)
		ret = true;
	else
	while (tmp != nullptr)
	{
		ret = tmp->data->Awake(config);
		tmp = tmp->next;
	}


	return ret;
}

bool j1EntityManager::Start()
{
	bool ret = false;

	//Load the texture to do the debug pathfinding
	debug_tex = App->tex->Load("maps/path2.png");

	p2List_item<j1Entity*>* tmp = EntityList.start;

	if (tmp == nullptr)
		ret = true;
	else
	while (tmp != nullptr)
	{
		ret = tmp->data->Start();
		tmp = tmp->next;
	}
	//Create the player
	AddEntity(entityType::PLAYER, { 0,0 });

	return ret;
}

bool j1EntityManager::PreUpdate()
{
	BROFILER_CATEGORY("Entity Manager Pre-Update", Profiler::Color::Blue)

	bool ret = false;
	p2List_item<j1Entity*>* tmp = EntityList.start;

	// Remove all entities scheduled for deletion
	while (tmp != nullptr)
	{
		if (tmp->data->collider->to_delete == true)
		{
			tmp->data->CleanUp(); //TODO: entities cleanUp + Collider
			RELEASE(tmp->data);
			EntityList.del(tmp);
			tmp = tmp->prev;
		}
		else
		tmp = tmp->next;
	}

	tmp = EntityList.start;

	if (tmp == nullptr)
		ret = true;
	else
	while (tmp != nullptr)
	{
		ret = tmp->data->PreUpdate();
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("Entity Manager Update", Profiler::Color::Blue)
	bool ret = false;
	p2List_item<j1Entity*>* tmp = EntityList.start;
	while (tmp != nullptr)
	{
		ret = tmp->data->Update(dt);
		tmp = tmp->next;
	}
	return ret;
}


bool j1EntityManager::PostUpdate()
{
	BROFILER_CATEGORY("Entity Manager Post-Update", Profiler::Color::Blue)

	bool ret = true;
	p2List_item<j1Entity*>* tmp = EntityList.start;
	while (tmp != nullptr)
	{
		if(tmp->data->type != entityType::PLAYER)
		tmp->data->PostUpdate(debug);
		else
		tmp->data->PostUpdate();


		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityManager::CleanUp()
{
	bool ret = true;

	App->tex->UnLoad(debug_tex);

	p2List_item<j1Entity*>* tmp = EntityList.start;
	while (tmp != nullptr)
	{
		tmp->data->CleanUp();
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityManager::Save(pugi::xml_node& file) const
{
	bool ret = true;
	p2List_item<j1Entity*>* tmp = EntityList.start;
	while (tmp != nullptr)
	{
		tmp->data->Save(file);
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityManager::Load(pugi::xml_node& file)
{
	bool ret = true;
	p2List_item<j1Entity*>* tmp = EntityList.start;
	while (tmp != nullptr)
	{
		tmp->data->Load(file);
		tmp = tmp->next;
	}
	return ret;
}

j1Entity* j1EntityManager::AddEntity(entityType type, iPoint position)
{
	j1Entity* tmp = nullptr;
	pugi::xml_document	config_file;
	pugi::xml_node		config;

	config = App->LoadConfig(config_file).child("entities");

	switch (type)
	{
	case entityType::PLAYER:
		if(player == nullptr)
		tmp = new j1Player();
		config = config.child("player");

		break;
	case entityType::FLYING_ENEMY:
		config = config.child("flyEnemy");

		break;
	case entityType::LAND_ENEMY:
		tmp = new j1LandEnemy();
		config = config.child("landEnemy");

		break;
	}
	if (tmp)
	{
		if (position.x != 0 && position.y != 0)
			tmp->SetPos(position.x, position.y);

		tmp->Awake(config);
		tmp->Start();

		EntityList.add(tmp);

	}

	return tmp;
}


bool j1EntityManager::InitEntity(j1Entity* tmp, pugi::xml_node& config)
{
	bool ret = false;

	ret = tmp->Awake(config);
	ret = tmp->Start();

	return ret;
}

bool j1EntityManager::CleanMapEnt()
{
	bool ret = false;

	p2List_item<j1Entity*>* tmp = EntityList.start;

	while (tmp != nullptr)
	{
		if (tmp->data->type != entityType::PLAYER)
		{
			tmp->data->CleanUp(); //TODO: entities cleanUp + Collider
			RELEASE(tmp->data);
			EntityList.del(tmp);
			tmp = tmp->prev;
		}
		else
			tmp = tmp->next;
	}

	return ret;
}

