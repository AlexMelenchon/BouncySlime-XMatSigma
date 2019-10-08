#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include "p2Point.h"


class j1Player : public j1Module
{
public:
	//Constructor
	j1Player();
//Destructor
	~j1Player();
	// Called before render is available
	bool Awake(pugi::xml_node&);
	// Called before the first frame
	bool Start();
	// Called each loop iteration
	bool PreUpdate();
	// Called each loop iteration
	bool Update(float dt);
	// Called each loop iteration
	bool PostUpdate();
	// Called before quitting
	bool CleanUp();
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

private:
	fPoint playerPos = {0,0}; //Determines player position on the map
	iPoint playerSpeed = {2,0}; // Determines player speed in the x and y axis

	SDL_Texture* playerTex = nullptr; //Player's spritesheet pointer
	SDL_Rect* idleRect = nullptr; // The player's rect in order to blit him

};

#endif 