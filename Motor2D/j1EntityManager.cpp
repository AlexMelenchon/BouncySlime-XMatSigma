#include "j1EntityManager.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"

//Constructor
j1EntityManager::j1EntityManager()
{
	name.create("entitycontroller");
}

//Destructor
j1EntityManager::~j1EntityManager()
{
}

// Called before render is available
bool j1EntityManager::Awake(pugi::xml_node& config)
{
	bool ret = false;

	AddEntity(entityType::PLAYER, { 0,0 });

	p2List_item<j1Entity*>* tmp = EntityList.start;
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

	p2List_item<j1Entity*>* tmp = EntityList.start;
	while (tmp != nullptr)
	{
		ret = tmp->data->Start();
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityManager::PreUpdate()
{
	bool ret = false;
	p2List_item<j1Entity*>* tmp = EntityList.start;

	// Remove all entities scheduled for deletion
	while (tmp != nullptr)
	{
		if (tmp->data->to_delete == true)
		{
			tmp = tmp->next;
			//TODO: func that releases enemy data in their cpp
			EntityList.del(tmp->prev);
		}
		else
		tmp = tmp->next;
	}

	while (tmp != nullptr)
	{
		ret = tmp->data->PreUpdate();
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityManager::Update(float dt)
{
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
	bool ret = true;
	p2List_item<j1Entity*>* tmp = EntityList.start;
	while (tmp != nullptr)
	{
		tmp->data->PostUpdate();
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityManager::CleanUp()
{
	bool ret = true;
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

	switch (type)
	{
	case entityType::PLAYER:
		//tmp = new Player();
		break;
	case entityType::FLYING_ENEMY:
		break;
	case entityType::LAND_ENEMY:
		break;
	}
	if (tmp)
		//	j1Entity.add(tmp);
		true;

	return tmp;
}
