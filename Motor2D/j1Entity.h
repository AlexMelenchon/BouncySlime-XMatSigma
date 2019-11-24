#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "p2Point.h"
#include "p2Log.h"
#include "j1App.h"
#include "p2SString.h"
#include "p2Defs.h"
#include "p2animation.h"

struct FX
{
	int id = 0;
	p2SString path;
};


enum collisionDirection
{
	DIRECTION_NONE = -1,

	DIRECTION_LEFT,
	DIRECTION_RIGHT,
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_MAX
};

enum  class entityType
{
	NO_TYPE,
	PLAYER,
	FLYING_ENEMY,
	LAND_ENEMY,
};


class j1Entity
{
public:
//FUNCTIONS-----------------------------

	//--------INTERNAL CONTROL---------//
	//Constructor
	j1Entity();

	//Secondary Constructor
	j1Entity(entityType type);

	//Destructor
	~j1Entity();

	// Called before render is available
	virtual bool Awake(pugi::xml_node&) { return true; };;

	// Called before the first frame
	virtual bool Start() { return true; };

	// Called each loop iteration
	virtual bool PreUpdate() { return true; };

	// Called each loop iteration
	virtual bool Update(float dt) { return true; };

	// Called each loop iteration
	virtual bool PostUpdate() { return true; };

	// Called before quitting
	virtual bool CleanUp() { return true;  };


	//--------SAVE & LOAD---------//
	//Called when loading a save
	virtual bool Load(pugi::xml_node&) { return true;  };

	//Called to save the game
	virtual bool Save(pugi::xml_node&) const  const { return true; };

	//--------POSITION ----------//
	// Limits the entity speed in both axis
	void LimitSpeed(float dt);

	//Set the entity's position from other modules such as j1Map
	virtual void SetPos(int x, int y);

	//Resets the entity's movement completly
	virtual void ReSetMovement();
	
	//Update entities's position
	virtual void UpdatePos(float dt) {};

	//--------COLLISION ---------//
	//Sets the collider to the player's position
	void CalculateCollider(fPoint);

	//If a collision is detected by the j1Collision, distributes collisions according to it's type
	virtual void OnCollision(Collider*, Collider*) {};

	//Calculate the collisions with the enviroment
	virtual void RecalculatePos(SDL_Rect, SDL_Rect) {};


	//--------STATE MACHiNE---------//
	//Updates the current state
	virtual void UpdateState() {};

	//--------DRAW---------//
	//Blits the entity into the world
	void Draw();

	//Calculates if the entity is flipped or not
	void FlipControl();

	//--------EXTERNAL---------//
	//Returns the entity's position
	fPoint getPos()
	{
		return fpPosition;
	}




//VARIABLES-----------------------------

	//--------ENTITY ---------//
	entityType type;

	//--------MOVEMENT ---------//
	//Determines entity position on the map
	fPoint fpPosition = { 0.0f,0.0f };

	// Determines player speed in the x and y axis
	fPoint fpSpeed = { 0.0f,0.0f };

	// Determines player acceleration in the x and y axis
	float fAccel = 0.0f;

	// Determines player acceleration in the x and y axis
	float fAccelX = 0.0f;

	float fGravity = 0.0f;

	//LIMITS---
	// Determines player maximum speed
	iPoint fpMaxSpeed = { 0.0f, 0.0f };

	//--------COLLISION ---------//
	//The entity's collider
	Collider* collider = nullptr;

	//--------RENDER---------//
	//Controls if the player is flipped or not
	SDL_RendererFlip Flip = SDL_RendererFlip::SDL_FLIP_NONE;

	//Player's spritesheet pointer
	SDL_Texture* Text = nullptr;

	//--------INTERNAL CONTROL---------//
	//To check if the entity is falling
	bool falling = false;

	// The speed needed so the sprite flips
	float flipSpeed = 0.0f;

	// Checks if the entity is scheduled to delete
	bool to_delete = false;

	//--------ANIMATIONS---------//
	Animation* currentAnimation = nullptr;

	//--------AUXILIAR---------//
	//Auxiliar node in order to be able to load the texture & rect
	pugi::xml_node auxLoader;
};

#endif