#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"



enum player_states
{
	ST_UNKNOWN,

	ST_GROUND,
	ST_AIR
};

enum player_inputs
{
	IN_UNKNOWN,
	IN_JUMP,
	IN_JUMP_FINISH
};

enum slow_direction
{
	SLOW_UNKNOWN,
	SLOW_GENERAL,
	SLOW_POSITIVE_X,
	SLOW_NEGATIVE_X
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

	void UpdatePos(float dt); //Update player's position
	void LimitPlayerSpeed();  // To limit the player speed in both axis
	void deAccel(slow_direction slow);  //To slow smoothly the player in the x axis: 0.Slow current speed 1. slow the x+, 2. slow the x-

	player_states process_fsm(p2List<player_inputs>& inputs);


private:
	p2List<player_inputs> inputs; //List that stores the players inputs
	player_states current_state = ST_UNKNOWN; //Player Starting state

	//Position variables
	fPoint fpPlayerPos = { 0.0f,0.0f }; //Determines player position on the map
	fPoint fpPlayerSpeed = { 0.0f,0.0f }; // Determines player speed in the x and y axis
	float fPlayerAccel = 0.0f; // Determines player acceleration in the x and y axis
	fPoint fpForce = { -180.0,-500.0f }; //Force applied to the player's movement in both axis

	//Position Limits
	float slowGrade = 1.25f;
	int slowLimit = 200;

	fPoint fpPlayerMaxSpeed = { 650.0f, 750.0f }; // Determines player maximum speed


	SDL_RendererFlip playerFlip = SDL_RendererFlip::SDL_FLIP_NONE; //Var that controls if the player is flipped or not

	SDL_Texture* playerTex = nullptr; //Player's spritesheet pointer
	SDL_Rect* idleRect = nullptr; // The player's rect in order to blit him


	//Internal control variables
	bool onPlatform = false;
	float flPreviousTime = 0;
	float flCurrentTime = 0;
	float fGravity = 50.0f;





	//Animations
	Animation* animPlayerIdle = nullptr;
};

#endif 