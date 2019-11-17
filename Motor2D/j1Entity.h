#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "p2Point.h"
#include "p2animation.h"
#include "p2Log.h"
#include "j1App.h"
#include "p2SString.h"

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


class Entity
{
public:
	enum entityType
	{
		NO_TYPE,
		PLAYER,
		FLYING_ENEMY,
		LAND_ENEMY,
	};

public:
	Entity();
	Entity(entityType type);
	~Entity();

	//--------INTERNAL CONTROL---------//
	//Constructor
	Entity();

	//Secondary Constructor
	Entity(entityType type);

	//Destructor
	~Entity();

	// Called before render is available
	virtual bool Awake(pugi::xml_node&) { return true; };;

	// Called before the first frame
	bool Start() { return true; };

	// Called each loop iteration
	bool PreUpdate() { return true; };

	// Called each loop iteration
	bool Update(float dt) { return true; };

	// Called each loop iteration
	bool PostUpdate() { return true; };

	// Called before quitting
	virtual bool CleanUp() {};

	//--------SAVE & LOAD---------//
	//Called when loading a save
	virtual bool Load(pugi::xml_node&) {};

	//Called to save the game
	virtual bool Save(pugi::xml_node&) const  const {};

	//--------POSITION ----------//
	//Update entity's position
	void UpdatePos(float dt);

	// Limits the entity speed in both axis
	void LimitSpeed(float dt);

	//Set the entity's position from other modules such as j1Map
	void SetPos(int x, int y);

	//Resets the entity's movement completly
	void ReSetMovement();


	//--------COLLISION ---------//
	//Sets the collider to the player's position
	void CalculateCollider(fPoint);

	//If a collision is detected by the j1Collision, distributes collisions according to it's type
	void OnCollision(Collider* c1, Collider* c2);

	//Calculate the collisions with the enviroment
	void RecalculatePos(SDL_Rect, SDL_Rect);


	//--------STATE MACHiNE---------//
	//Updates the current state
	void UpdateState();

private:
	//--------ENTITY ---------//
	entityType type;

	//--------MOVEMENT ---------//
	//Determines entity position on the map
	fPoint fpPosition = { 0.0f,0.0f };

	// Determines player speed in the x and y axis
	fPoint fpSpeed = { 0.0f,0.0f };

	// Determines player acceleration in the x and y axis
	float fAccel = 0.0f;

	float fGravity = 0.0f;

	//LIMITS---
	// Determines player maximum speed
	iPoint fpMaxSpeed = { 0.0f, 0.0f };

	//--------COLLISION ---------//
	//The entity's collider
	Collider* Collider = nullptr;

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

	//--------ANIMATIONS---------//
	Animation* currentAnimation = nullptr;

	//--------AUXILIAR---------//
	//Auxiliar node in order to be able to load the texture & rect
	pugi::xml_node auxLoader;
};

#endif