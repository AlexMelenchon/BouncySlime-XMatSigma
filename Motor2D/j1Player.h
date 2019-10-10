#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include "p2Point.h"
#include "SDL/include/SDL_render.h"
#include "p2List.h"
#include "p2Log.h"



#define JUMPINGFRAMES 60
#define JUMP_INIT_VY 10 //initial velocity on the Y coord. //16.2
#define JUMP_INIT_AY 0.5 //initial acceleration on the Y coord. //0.867

enum player_states
{
	ST_UNKNOWN,

	ST_IDLE,
	ST_WALK_FORWARD,
	ST_WALK_BACKWARD,
	ST_JUMP,
	ST_JUMP_FORWARD,
	ST_JUMP_BACKWARD
};

enum player_inputs
{
	IN_UNKNOWN,
	IN_LEFT,
	IN_RIGHT,
	IN_LEFT_AND_RIGHT,
	IN_JUMP,
	IN_JUMP_FINISH
};

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

	void UpdatePos(float dt);
	void LimitPlayerSpeed();

	player_states process_fsm(p2List<player_inputs>& inputs);


private:
	p2List<player_inputs> inputs; //List that stores the players inputs
	player_states current_state = ST_UNKNOWN; //Player Starting state

	//Position variables
	fPoint fpPlayerPos = { 0.0f,0.0f }; //Determines player position on the map
	fPoint fpPlayerSpeed = { 0.0f,0.0f }; // Determines player speed in the x and y axis
	fPoint fpPlayerAccel = { 0.0f,0.0f }; // Determines player acceleration in the x and y axis
	fPoint fpForce = { -15,-10.f };


	fPoint fpPlayerMaxSpeed = { 50.0f, 50.0f }; // Determines player maximum speed


	SDL_RendererFlip playerFlip = SDL_RendererFlip::SDL_FLIP_NONE; //Var that controls if the player is flipped or not

	SDL_Texture* playerTex = nullptr; //Player's spritesheet pointer
	SDL_Rect* idleRect = nullptr; // The player's rect in order to blit him


	//Internal control variables
	bool onPlatform = false;
	float flPreviousTime = 0;
	float flCurrentTime = 0;

};

#endif 