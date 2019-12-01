#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Entity.h"

enum player_states
{
	ST_UNKNOWN,

	ST_GROUND,
	ST_AIR,
	ST_FALLING,
	ST_WALL,
	ST_WALL_JUMPING,
	ST_GOD,
	ST_DEAD
};

enum player_inputs
{
	IN_UNKNOWN,
	IN_JUMP,
	IN_JUMP_FINISH,
	IN_FALL,
	IN_WALL,
	IN_JUMP_WALL,
	IN_GOD,
	IN_DEATH,
	IN_DEATH_FINISH
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

class j1Player : public j1Entity
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

	//Smoothly slows an speed axis
	float deAccel(slow_direction slow, float speedAxis, float grade = 0.0f, float limit = 0.0f);  

	//Resets the player's movement completly
	void ReSetMovement();

	//Set the entity's position from other modules such as j1Map
	void SetPos(int x, int y);

	//The action of jumping
	void Jump(float forceY, int fxId);


	//--------COLLISION ---------//
	//If a collision is detected by the j1Collision, distributes collisions according to it's type
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

	//When the player's state turns wall: reset everything & limit it's entrance speed
	void InWall();

	//Iterates the states taking in account the inputs
	player_states process_fsm(p2List<player_inputs>& inputs); 

	//--------EXTERNAL---------//
	//Returns the player's state
	player_states getState()
	{
		return current_state;
	}

	//--------STATE MACHiNE---------//
	//List that stores the players inputs
	p2List<player_inputs> inputs;
private:

	//--------MOVEMENT ---------//
	//Force applied to the player's movement in both axis
	fPoint fpForce = { 0.0f,0.0f }; 

	//Force applied to the player's when killing an enemy
	fPoint fpForceMiniJump = { 0.0f,0.0f };

	//Force applied to the player when jumping off a wall
	fPoint wallForce = { 0.0f, 0.0f };
	
	//Used to have an smooth leave of the wall
	float wallingLeave = 0.0f;

	//LIMITS---
	//The amount in which a speed will be divided to per frame.
	float fSlowGrade = 0.0f;

	//The amount in which a speed will be divided to per frame in hte air
	float fSlowGradeAir = 0.0f;

	//The amount in which a speed will be divided to per frame.
	float fSlowGradeWall = 0.0f;

	//The limit in order to perfom an slow in a speed
	int iSlowLimit = 0;

	//The amount that the speed is reduced entering a wall
	int inWallSpeedDrop = 0;

	//--------INTERNAL CONTROL---------//
	// Actual time of the ms elapsed since the last frame
	float flCurrentTime = 0.0f;

	// Fade time to change maps or death
	float playerFadeTime = 0.0f;

	// To check if the player is near a wall
	bool walling = false;

	// The thime limit to restrain the player's movement will be restrained after jumping of a wall
	float wallJumpLimit = 0.0f;

	// Controls the time elapsed since the player jumped of a wall
	float wallJumpTimer = 0.0f;

	// Controls the time elapsed since the player wants to leave the wall and actually leaves (jump breathing room)
	float wallJumpLeaveControl = 0.0f;

	float godSpeedMultiplier = 0.0f;

	//The player's direction when jumping off a wall
	collisionDirection wallJumpDirection = DIRECTION_NONE;

	//Controls godMode
	bool god = false; 

	//--------ANIMATIONS---------//
	Animation animIdle;
	Animation animJump;
	Animation animRun;
	Animation animFall;
	Animation animWall;
	Animation animDeath;
	
	//--------MUSIC---------//
	FX jumpFx;
	FX deathFx;
	FX landFx;
	FX winFx;
	FX bounceFx;
	FX enemyDeathFx;
	
	//Player Starting state
	player_states current_state = ST_UNKNOWN; 
};

#endif 
