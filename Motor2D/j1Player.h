#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"

struct FX
{
	int id = 0;
	p2SString path;
};
enum player_states
{
	ST_UNKNOWN,

	ST_GROUND,
	ST_AIR,
	ST_FALLING,
	ST_WALL,
	ST_WALL_JUMPING,
	ST_GOD
};

enum player_inputs
{
	IN_UNKNOWN,
	IN_JUMP,
	IN_JUMP_FINISH,
	IN_FALL,
	IN_WALL,
	IN_JUMP_WALL,
	IN_GOD
};

enum slow_direction
{
	SLOW_UNKNOWN,
	SLOW_GENERAL,
	SLOW_AIR,
	SLOW_POSITIVE_LIMIT,
	SLOW_NEGATIVE_LIMIT,
	SLOW_LIMITS
};

enum collisionDirection 
{
	DIRECTION_NONE = -1,

	DIRECTION_LEFT,
	DIRECTION_RIGHT,
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_MAX
};

class j1Player : public j1Module
{
public:
	//--------INTERNAL CONTROL---------//
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


	//--------SAVE & LOAD---------//
	//Called when loading a save
	bool Load(pugi::xml_node&);

	//Called to save the game
	bool Save(pugi::xml_node&) const;


	//--------POSITION ----------//
	//Update player's position
	void UpdatePos(float dt); 

	// Limits the player speed in both axis
	void LimitPlayerSpeed();  

	//Smoothly slows an speed axis
	float deAccel(slow_direction slow, float speedAxis, float grade = 0.0f, float limit = 0.0f);  

	//Set the player's position from other modules such as j1Map
	void SetPos(int x, int y);

	//Resets the player's movement completly
	void ReSetMovement();


	//--------COLLISION ---------//
	//Sets the collider to the player's position
	void CalculateCollider(fPoint);

	//If a collision is detected by the j1Collision, distributes collisions
	void OnCollision(Collider* c1, Collider* c2);

	//Calculate the collisions with the enviroment
	void RecalculatePos(SDL_Rect, SDL_Rect);


	//--------INPUTS---------//
	//Standard Movement
	void standardInputs();

	//Controls in God Mode movement
	void godInputs(); 

	//Controls God Mode
	void GodMode(); 


	//--------STATE MACHiNE---------//
	//Updates the current state
	void UpdateState();

	//When the player's state turns ground
	void inGround();

	//When the player's state turns wall
	void InWall();

	//Iterates the states taking in account the inputs
	player_states process_fsm(p2List<player_inputs>& inputs); 


	//--------EXTERNAL---------//
	//Returns the player's position
	fPoint getPos()
	{
		return fpPlayerPos;
	}

	//Returns the player's state
	player_states getState()
	{
		return current_state;
	}


private:

	//--------MOVEMENT ---------//
	//Determines player position on the map
	fPoint fpPlayerPos = { 0.0f,0.0f }; 

	// Determines player speed in the x and y axis
	fPoint fpPlayerSpeed = { 0.0f,0.0f }; 

	// Determines player acceleration in the x and y axis
	float fPlayerAccel = 0.0f; 

	//Force applied to the player's movement in both axis
	fPoint fpForce = { 0.0f,0.0f }; 

	//Force applied to the player when jumping off a wall
	fPoint wallForce = { 0.0f, 0.0f };

	float fGravity = 0.0f;

	//LIMITS---
	//The amount in which a speed will be divided to per frame.
	float fSlowGrade = 0.0f;

	//The amount in which a speed will be divided to per frame in hte air
	float fSlowGradeAir = 0.0f;

	//The amount in which a speed will be divided to per frame.
	float fSlowGradeWall = 0.0f;


	//The limit in order to perfom an slow in a speed
	int iSlowLimit = 0; 

	// Determines player maximum speed
	iPoint fpPlayerMaxSpeed = { 0.0f, 0.0f }; 


	//--------COLLISION ---------//
	//The player's collider
	Collider* playerCollider = nullptr; 


	//--------RENDER---------//
	//Controls if the player is flipped or not
	SDL_RendererFlip playerFlip = SDL_RendererFlip::SDL_FLIP_NONE; 

	//Player's spritesheet pointer
	SDL_Texture* playerTex = nullptr; 


	//--------INTERNAL CONTROL---------//
	// Prrevious time of the ms elapsed since the last frame
	float flPreviousTime = 0.0f; 

	// Actual time of the ms elapsed since the last frame
	float flCurrentTime = 0.0f;

	// Limits the time elapsed between frames so the movement doesn't break
	float fInFramesLimit = 0.0f;

	// Fade time to change maps or death
	float playerFadeTime = 0.0f;

	// The amount of force needed to be able to fall from a wall
	float wallForceToMove = 0.0f;

	//To check if the player is falling
	bool falling = false;

	// To check if the player is near a wall
	bool walling = false;

	// The thime limit to restrain the player's movement will be restrained after jumping of a wall
	float wallJumpLimit = 0.0f;

	// Controls the time elapseed since the player jumped of a wall
	float wallJumpTimer = 0.0f;

	//The player's direction when jumping off a wall
	collisionDirection wallJumpDirection = DIRECTION_NONE;

	//Controls godMode
	bool god = false; 


	//--------ANIMATIONS---------//
	Animation* currentAnimation = nullptr;
	Animation animIdle;
	Animation animJump;
	Animation animRun;
	Animation animFall;
	Animation animWall;
	
	//--------MUSIC---------//
	FX jumpFx;
	FX deathFx;
	FX landFx;
	FX winFx;
	FX bounceFx;

	//--------STATE MACHiNE---------//
	//List that stores the players inputs
	p2List<player_inputs> inputs; 

	//Player Starting state
	player_states current_state = ST_UNKNOWN; 


	//--------AUXILIAR---------//
	//Auxiliar node in order to be able to load the texture & rect
	pugi::xml_node auxLoader;


	int pivotX;
	int pivotY;
};

#endif 