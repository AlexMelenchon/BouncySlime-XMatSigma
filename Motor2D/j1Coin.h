#ifndef __j1COIN_H__
#define __j1COI_H__

#include "j1Entity.h"
#include "p2Defs.h"
#include "p2List.h"
#include "p2Point.h"
#include "p2DynArray.h"

class j1Coin : public j1Entity
{
public:

	//--------INTERNAL CONTROL---------//
	//Constructor
	j1Coin();

	//Destructor
	~j1Coin();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called each loop iteration
	bool PostUpdate(bool debug);

	// Called before quitting

	bool CleanUp();
	//--------SAVE & LOAD---------//
	//Called when loading a save
	bool Load(pugi::xml_node&);

	//Called to save the game
	bool Save(pugi::xml_node&) const;

	//--------COLLISION ---------//
	//If a collision is detected by the j1Collision, distributes collisions according to it's type
	void OnCollision(Collider* coinCol, Collider* coll);


private:
	//--------ANIMATION ---------//
	Animation anim;

};


#endif

