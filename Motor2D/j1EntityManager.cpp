#include "j1EntityManager.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1LandEnemy.h"
#include "j1FlyingEnemy.h"
#include "j1Map.h"
#include "j1Collision.h"
#include "j1ParticleShuriken.h"	

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
	
	//We save the node to later use in the code
	entConfig = config;

	fInFramesLimit = config.child("manager").child("inFramesLimit").text().as_float();

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
			tmp->data->CleanUp();
			RELEASE(tmp->data);
			EntityList.del(tmp);
			tmp = tmp->prev;
		}
		else
		tmp = tmp->next;
	}


	//Pre-update (inputs & logic) of all entities
	tmp = EntityList.start;

	//If no entity is loaded, we do not do nothing
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

// Called each loop iteration
bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("Entity Manager Update", Profiler::Color::Blue)
	bool ret = false;
	p2List_item<j1Entity*>* tmp = EntityList.start;

	//If the game is paused, we do not calculate the logic
	if (dt == 0.0f) return true;

	//A cap to prevent the game logic from going crazy in specific & critical conditions (ex. big time load)
	if (dt > fInFramesLimit)
		dt = fInFramesLimit;

	//We update all the entities
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

	CleanMapEnt();

	p2SString playerStr("player");
	p2SString enemyL("enemyL");
	p2SString enemyF("enemyF");

	for (pugi::xml_node propIterator = file.child("enemyL"); propIterator; propIterator = propIterator.next_sibling())
	{
		if (enemyL == propIterator.name())
			AddEntity(entityType::LAND_ENEMY, { 0,0 })->Load(propIterator);

	}

	for (pugi::xml_node propIterator = file.child("enemyF"); propIterator; propIterator = propIterator.next_sibling())
	{

		if (enemyF == propIterator.name())
			AddEntity(entityType::FLYING_ENEMY, { 0,0 })->Load(propIterator);

	}

	player->Load(file.child("player"));


	return ret;
}

j1Entity* j1EntityManager::AddEntity(entityType type, iPoint position, iPoint movement)
{
	j1Entity* tmp = nullptr;
	pugi::xml_node config;

	switch (type)
	{
	case entityType::PLAYER:
		if(player == nullptr)
		tmp = new j1Player();
		config = entConfig.child("player");

		break;
	case entityType::FLYING_ENEMY:
		tmp = new j1FlyingEnemy();
		config = entConfig.child("flyingenemy");

		break;
	case entityType::LAND_ENEMY:
		tmp = new j1LandEnemy();
		config = entConfig.child("landenemy");

		break;
	case entityType::SHURIKEN:
		tmp = new j1ParticleShuriken();
		config = entConfig.child("shuriken");
		break;
	}
	if (tmp)
	{
		if (position.x != 0 && position.y != 0)
		{
			tmp->SetPos(position.x, position.y);

			if (tmp->type == entityType::LAND_ENEMY || tmp->type == entityType::FLYING_ENEMY)
			{
				tmp->SetTrace({ position.x, position.y, movement.x, movement.y });
			}
				
		}
			

		InitEntity(EntityList.add(tmp)->data, config);

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