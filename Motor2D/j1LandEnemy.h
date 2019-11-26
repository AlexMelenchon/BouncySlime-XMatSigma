#ifndef __j1LANDENEMY_H__
#define __j1LANDENEMY_H__

#include "j1Entity.h"
#include "p2Defs.h"
#include "p2List.h"
#include "p2Point.h"
#include "p2DynArray.h"

#define CHASING_DISTANCE 200

 enum class state : int
{
	 ST_IDLE,
	 ST_CHASING,
	 ST_UNKNOWN,
};


class j1LandEnemy : public j1Entity
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
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called each loop iteration
	bool PostUpdate(bool debug);

	// Called before quitting
	bool CleanUp();

	//--------POSITION ---------//
	void UpdatePos(float dt);

	//--------COLLISION ---------//
	
	//If a collision is detected by the j1Collision, distributes collisions according to it's type
	void OnCollision(Collider* playerCol, Collider* coll);

	//Calculate the collisions with the enviroment
	void RecalculatePos(SDL_Rect, SDL_Rect);


private:
	//--------MOVEMENT ---------//
	bool GetPathfinding();

	//--------INTERNAL CONTROÑ ---------//
	float pathTimer;
	
	//--------STATE ---------//
	//determines what the enemy is doing
	void UpdateState(); 
	state enemy_state;
	p2DynArray<iPoint> path;

	//--------ANIMATIONS---------//
	Animation animIdle;
	Animation animJump;
	Animation animRun;
	Animation animFall;
	Animation animWall;
	Animation animDeath;
	
};


#endif // !