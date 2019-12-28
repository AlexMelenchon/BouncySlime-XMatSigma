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

	//Determines if we are in debug mode
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

	//Cleans all the map entities
	bool CleanMapEnt();

	//Erases the player from the map
	bool DeletePlayer();

	//Since player is used in other modules, we are going to have a pointer only to him
	//Also the player is an unique entity case in the code, since is never destroyed,
	//we will only change it's position & reset it's movement & state
	j1Player* player = nullptr;

	//Since the shuriken is used by the player and we can only have one of each, we track the one we create with a pointer
	j1ParticleShuriken* shuriken = nullptr;

	//Node entity configuration
	pugi::xml_node		entConfig;

	//--------PATHFINDING---------//
	SDL_Texture* debug_tex;

	//--------INTERNAL CONTROL---------//
	//A cap to prevent the game logic from going crazy in specific & critical conditions (ex. big time load)
	float fInFramesLimit = 0.0f;

	//--------TEXTURES---------//
	//We load the enemy textures here, so there are not multiple loads
	SDL_Texture* flying_tex = nullptr;
	SDL_Texture* land_tex = nullptr;
	SDL_Texture* player_tex = nullptr;


private:
	//--------ENTITIES---------//
	//The list where we will store all the entities
	p2List<j1Entity*> EntityList;
};
#endif