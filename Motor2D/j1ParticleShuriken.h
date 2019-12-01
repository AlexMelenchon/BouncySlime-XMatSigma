#ifndef __j1SHURIKENPARTICLE_H__
#define __j1SHURIKENPARTICLE_H__

#include "j1Entity.h"
#include "p2Defs.h"
#include "p2List.h"
#include "p2Point.h"
#include "p2DynArray.h"

class j1ParticleShuriken : public j1Entity
{
public:

	//--------INTERNAL CONTROL---------//
	//Constructor
	j1ParticleShuriken();

	//Destructor
	~j1ParticleShuriken();

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

	void CheckDir();


	//--------COLLISION ---------//

	//If a collision is detected by the j1Collision, distributes collisions according to it's type
	void OnCollision(Collider* playerCol, Collider* coll);

	//Calculate the collisions with the enviroment
	void RecalculatePos(SDL_Rect, SDL_Rect);

	bool ReturnToPlayerPath();

	void Return(float dt);

	//--------DRAW ---------//
	void Draw();

private:	
	Animation anim;

	//movement
	p2DynArray<iPoint> path;
	float speedReduction = 0.f;
	float returnTimer = 0.f;
	float minimumSpeed = 0.f;
	float friction = 0,f;
	fPoint returnSpeed = { 0.f,0.f };
	float defaultSpeed = 0.f;


	//fx
	FX shuriken_hit;
	FX in_air;


	//internal
	bool canPickUp = false;
	float timer = 0.0f;

};


#endif


