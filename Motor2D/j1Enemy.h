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
	//Update enemy's position
	virtual void UpdatePos(float dt) {};

	//When the enemy is idle, it has a defined movement (a.k.a patrol)
	virtual void TraceFollower() {};

	//Moves the enemy according to pathfinding
	virtual void Move(float dt) {};

	//--------COLLISION ---------//

	//If a collision is detected by the j1Collision, distributes collisions according to it's type
	virtual void OnCollision(Collider* playerCol, Collider* coll) {};

	//Calculate the collisions with the enviroment
	virtual void RecalculatePos(SDL_Rect, SDL_Rect) {};

	//--------PATHFINDING---------//
	//Return the pathfinding to a map destination, also can be cap or not
	bool GetPathfinding(fPoint destination, bool tilelimit = true);


protected:
	//--------MOVEMENT ---------//

	//Determines enemy patrol direction
	collisionDirection traceDir = DIRECTION_RIGHT;

	//The enemies' sight range
	uint chasingDistance = 0u;

	//The enemies' chase max distance in tiles
	uint chasingTiles = 0u;

	//Timers for pathfinding
	float idleTimer = 0.f;
	float chasingTimer = 0.f;

	//Determines an entity predefined movement
	SDL_Rect trace = { 0,0,0,0 };

	//defined movement speed the enemy will have
	fPoint idleSpeed = { 0,0 };
	fPoint moveSpeed = { 0,0 };

	//--------STATE ---------//

	//Enemy current state
	enemy_state state;

	//Changes the state of the enemy (it's not a state machine like the player, since the enemy states are minimum & simple)
	void UpdateState();

	//Array that stores enemy pathfinding
	p2DynArray<iPoint> path;


};


#endif // !
