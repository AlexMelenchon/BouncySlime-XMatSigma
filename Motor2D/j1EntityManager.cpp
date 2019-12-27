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
	bool ret = true;

	//We save the node to later use in the code
	entConfig = config;

	//Variable load------------------------
	fInFramesLimit = config.child("manager").child("inFramesLimit").text().as_float();

	return ret;
}

// Called before the first frame
bool j1EntityManager::Start()
{
	bool ret = true;

	//Load the texture to do the debug pathfinding
	debug_tex = App->tex->Load(entConfig.child("manager").child("debugTextPath").text().as_string());
	flying_tex = App->tex->Load(entConfig.child("flyingenemy").child("path").text().as_string());
	land_tex = App->tex->Load(entConfig.child("landenemy").child("path").text().as_string());




	return ret;
}

// Called each loop iteration
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


	//We return to the start of the lsit & iterate throug them all, doing their respective pre-update
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
		//If no entity is loaded, we do not do nothing
	if (tmp == nullptr)
		ret = true;
	else
		while (tmp != nullptr)
		{
			ret = tmp->data->Update(dt);
			tmp = tmp->next;
		}
	return ret;
}

// Called each loop iteration
bool j1EntityManager::PostUpdate()
{
	BROFILER_CATEGORY("Entity Manager Post-Update", Profiler::Color::Blue)

	bool ret = true;

	//We iterate though all the entities & we do their respective postupdate
	p2List_item<j1Entity*>* tmp = EntityList.start;
	while (tmp != nullptr)
	{
		if (tmp->data->type != entityType::PLAYER)
			tmp->data->PostUpdate(debug);
		else
			tmp->data->PostUpdate();


		tmp = tmp->next;
	}

	return ret;
}

// Called before quitting
bool j1EntityManager::CleanUp()
{
	bool ret = true;

	//Texture clean
	App->tex->UnLoad(debug_tex);
	App->tex->UnLoad(flying_tex);
	App->tex->UnLoad(land_tex);

	//Iterate though all the remaining entities cleanUp
	p2List_item<j1Entity*>* tmp = EntityList.start;
	while (tmp != nullptr)
	{
		tmp->data->CleanUp();
		RELEASE(tmp->data);
		EntityList.del(tmp);
		tmp = tmp->next;
	}

	EntityList.clear();

	return ret;
}

//Called to save the game
bool j1EntityManager::Save(pugi::xml_node& file) const
{
	bool ret = true;

	//Iterate though all the entities' save
	p2List_item<j1Entity*>* tmp = EntityList.start;
	while (tmp != nullptr)
	{
		tmp->data->Save(file);
		tmp = tmp->next;
	}
	return ret;
}

//Called when loading a save
bool j1EntityManager::Load(pugi::xml_node& file)
{
	bool ret = true;

	//First of all, we clean all the entities that are not the player, if any
	CleanMapEnt();

	//Create the string to compare
	p2SString playerStr("player");
	p2SString enemyL("enemyL");
	p2SString enemyF("enemyF");

	//We iterate though all the entities that we saved, creating each one of them
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

	//Also, we load the player :)
	player->Load(file.child("player"));


	return ret;
}

//Function that adds entities
j1Entity* j1EntityManager::AddEntity(entityType type, iPoint position, iPoint movement)
{
	j1Entity* tmp = nullptr;
	pugi::xml_node config;

	//We determine which type is our entity, allocate mem to it & adjust the loading node; if exists
	switch (type)
	{
	case entityType::PLAYER:
		if (player == nullptr)
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

	case entityType::COIN:
		tmp = new j1ParticleShuriken();
		config = entConfig.child("coin");

		break;
	}

	//If there was a succesful match, we create the entity
	if (tmp)
	{
		if (position.x != 0 && position.y != 0)
		{
			//We set the position
			tmp->SetPos(position.x, position.y);

			//If the entity is an enemy, we also set it's trace (a.k.a it's patrol path)
			if (tmp->type == entityType::LAND_ENEMY || tmp->type == entityType::FLYING_ENEMY)
			{
				tmp->SetTrace({ position.x, position.y, movement.x, movement.y });
			}

		}

		//We awake & init the entity we just loaded
		InitEntity(EntityList.add(tmp)->data, config);

	}

	return tmp;
}

//Function that initializes entities
bool j1EntityManager::InitEntity(j1Entity* tmp, pugi::xml_node& config)
{
	bool ret = false;

	//We awake & start the entities
	ret = tmp->Awake(config);
	ret = tmp->Start();

	return ret;
}

//Cleans all the map entities
bool j1EntityManager::CleanMapEnt()
{
	bool ret = false;

	//We iterate though all the entities & destroy 'em all! Except the player
	p2List_item<j1Entity*>* tmp = EntityList.start;
	while (tmp != nullptr)
	{
		if (tmp->data->type != entityType::PLAYER)
		{
			tmp->data->CleanUp();
			RELEASE(tmp->data);
			EntityList.del(tmp);
		}
		tmp = tmp->next;

	}

	return ret;
}


//Erases the player
bool j1EntityManager::DeletePlayer()
{
	bool ret = false;

	//We iterate until we find the player and delete it
	p2List_item<j1Entity*>* tmp = EntityList.start;
	while (tmp != nullptr)
	{
		if (tmp->data->type == entityType::PLAYER)
		{
			tmp->data->CleanUp();
			RELEASE(tmp->data);
			EntityList.del(tmp);
			break;
		}
		tmp = tmp->next;

	}
	EntityList.clear();

	return ret;
}