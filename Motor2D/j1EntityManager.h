#ifndef __j1ENTITYMANAGER_H__
#define __j1ENTITYMANAGER_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "j1Player.h"
#include "j1LandEnemy.h"
#include "j1ParticleShuriken.h"
#include "p2Defs.h"
#include "j1Entity.h"

class j1Entity;


class j1EntityManager : public j1Module
{
	//------------Methods-----------//
public:
	//--------INTERNAL CONTROL---------//
	//Constructor
	j1EntityManager();

	//Destructor
	~j1EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called each loop iteration
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool debug = false;


	//--------SAVE & LOAD---------//
	//Called when loading a save
	bool Load(pugi::xml_node&);

	//Called to save the game
	bool Save(pugi::xml_node&) const;

	//--------ENTITY MANAGEMENT---------//
	//Function that adds entities
	j1Entity* AddEntity(entityType type, iPoint position, iPoint movement = { 0,0 });

	//Function that initializes entities
	bool InitEntity(j1Entity* tmp, pugi::xml_node& config);

	//Function that initializes entities
	bool CleanMapEnt();

	//Since player is used in other modules, we are going to have a pointer only to him
	j1Player* player = nullptr;	

	//Node entity configuration
	pugi::xml_node		entConfig;

	//--------PATHFINDING---------//
	SDL_Texture* debug_tex;

private:
	p2List<j1Entity*> EntityList;
};
#endif