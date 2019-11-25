//#ifndef __j1FLYINGENEMY_H__
//#define __j1FLYINGENEMY_H__
//
//#include "j1Entity.h"
//
//
//class j1FlyingEnemy : public j1Entity
//{
//public:
//	//--------INTERNAL CONTROL---------//
//	//Constructor
//	j1FlyingEnemy();
//
//	//Destructor
//	~j1FlyingEnemy();
//
//	// Called before render is available
//	bool Awake(pugi::xml_node&);
//
//	// Called before the first frame
//	bool Start();
//
//	// Called each loop iteration
//	bool PreUpdate();
//
//	// Called each loop iteration
//	bool Update(float dt);
//
//	// Called each loop iteration
//	bool PostUpdate();
//
//	// Called before quitting
//	bool CleanUp();
//
//
//	//--------SAVE & LOAD---------//
//	//Called when loading a save
//	bool Load(pugi::xml_node&);
//
//	//Called to save the game
//	bool Save(pugi::xml_node&) const;
//
//	//--------POSITION ----------//
//	//Update player's position
//	void UpdatePos(float dt);
//
//	//--------COLLISION ---------//
//	//If a collision is detected by the j1Collision, distributes collisions according to it's type
//	void OnCollision(Collider* c1, Collider* c2);
//
//	//Calculate the collisions with the enviroment
//	void RecalculatePos(SDL_Rect, SDL_Rect);
//
//
//private:
//
//	//--------MOVEMENT ---------//
//	//Force applied to the player's movement in both axis
//	fPoint fpForce = { 0.0f,0.0f };
//
//
//
//	//--------ANIMATIONS---------//
//	Animation animIdle;
//	Animation animJump;
//	Animation animRun;
//	Animation animFall;
//	Animation animWall;
//	Animation animDeath;
//
//	//--------MUSIC---------//
//	FX jumpFx;
//	FX deathFx;
//	FX landFx;
//	FX winFx;
//	FX bounceFx;
//}
//
//
//
//#endif