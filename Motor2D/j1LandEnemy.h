#ifndef __j1LANDENEMY_H__
#define __j1LANDENEMY_H__

#include "j1Entity.h"
#include "p2Defs.h"
#include "p2List.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1Enemy.h"

class j1LandEnemy : public j1Enemy
{
public: 	
	
	//--------INTERNAL CONTROL---------//
	//Constructor
	j1LandEnemy();

	//Destructor
	~j1LandEnemy();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	//--------SAVE & LOAD---------//
	//Called when loading a save
	bool Load(pugi::xml_node&);

	//Called to save the game
	bool Save(pugi::xml_node&) const;

	//--------POSITION ---------//
	void UpdatePos(float dt);

	//When the enemy is idle, it has a defined movement
	void TraceFollower();

	//Gets if the next tile, in the enemy's foot is walkable
	iPoint NextWalkableTile();

	iPoint AbleToMove();

	int CalculateJump(iPoint destination);

	bool JumpLogic();

	void Move(bool toPlayer, float dt);

	//--------COLLISION ---------//
	
	//If a collision is detected by the j1Collision, distributes collisions according to it's type
	void OnCollision(Collider* playerCol, Collider* coll);

	//Calculate the collisions with the enviroment
	void RecalculatePos(SDL_Rect, SDL_Rect);

private:
	//--------MOVEMENT ---------//
	float jumpForce = 0.f;
	float jumpDistance = 0.f;
	float fallingSpeed = 0.f;

	//--------INTERNAL CONTROÑ ---------//
	float pathTimer = 0.0f;
	float timer = 0.f;
	bool stop = false;
	
	//--------ANIMATIONS---------//
	Animation animIdle;
	Animation animJump;
	Animation animRun;
	Animation animFall;
	Animation animWall;
	Animation animDeath;
	SDL_Rect enemyRect = { 0,0,0,0 };
	
};


#endif // !