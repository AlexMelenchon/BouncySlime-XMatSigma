#ifndef __j1ENEMY_H__
#define __j1ENEMY_H__

#include "j1Entity.h"
#include "p2Defs.h"
#include "p2List.h"
#include "p2Point.h"
#include "p2DynArray.h"

#define CHASING_DISTANCE 450
#define CHASING_MAX_TILES 32


enum class enemy_state : int
{
	ST_UNKNOWN = -1,

	ST_IDLE,
	ST_CHASING,

	ST_ALL,
};


class j1Enemy : public j1Entity
{
public:

	//--------INTERNAL CONTROL---------//
	//Constructor
	j1Enemy();

	//Destructor
	~j1Enemy();

	// Called before render is available
	virtual bool Awake(pugi::xml_node&) { return true; };

	// Called before the first frame
	virtual bool Start() { return true; };

	// Called each loop iteration
	bool PreUpdate();

	// Called each loop iteration
	virtual bool Update(float dt) { return true; };

	// Called each loop iteration
	bool PostUpdate(bool debug);

	// Called before quitting
	bool CleanUp();


	//--------SAVE & LOAD---------//
	//Called when loading a save
	virtual bool Load(pugi::xml_node&) { return true; };

	//Called to save the game
	virtual bool Save(pugi::xml_node&) const {return true; }

	//--------POSITION ---------//
	virtual void UpdatePos(float dt) {};

	//When the enemy is idle, it has a defined movement
	virtual void TraceFollower() {};

	virtual void Move(float dt) {};

	//--------COLLISION ---------//

	//If a collision is detected by the j1Collision, distributes collisions according to it's type
	virtual void OnCollision(Collider* playerCol, Collider* coll) {};

	//Calculate the collisions with the enviroment
	virtual void RecalculatePos(SDL_Rect, SDL_Rect) {};

	//--------PATHFINDING---------//
	bool GetPathfinding(fPoint destination, bool tilelimit = true);


public:
	//--------MOVEMENT ---------//
	collisionDirection traceDir = DIRECTION_RIGHT;

	//--------STATE ---------//
	//determines what the enemy is doing
	enemy_state state;
	void UpdateState();
	p2DynArray<iPoint> path;


};


#endif // !
