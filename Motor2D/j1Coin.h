#ifndef __j1SHURIKENPARTICLE_H__
#define __j1SHURIKENPARTICLE_H__

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
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called each loop iteration
	bool PostUpdate(bool debug);

	// Called before quitting
	bool CleanUp();


	//--------COLLISION ---------//

	//If a collision is detected by the j1Collision, distributes collisions according to it's type
	void OnCollision(Collider* playerCol, Collider* coll);


private:
	//--------ANIMATION ---------//
	Animation anim;

};


#endif

