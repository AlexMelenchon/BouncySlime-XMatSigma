#include "j1Player.h"
#include "j1Input.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1FadeToBlack.h"
#include "j1Map.h"
#include "j1Audio.h"


//Constructor
j1Player::j1Player() : j1Module()
{
	name.create("player");
}

// Destructor
j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& player_node)
{
	//State Machine start
	inputs.start = 0; 
	
	//Movement load
	fpForce.x = player_node.child("movement").child("movementForce").attribute("x").as_float();
	fpForce.y = player_node.child("movement").child("movementForce").attribute("y").as_float();

	wallForce.x = player_node.child("movement").child("wallForce").attribute("x").as_float();
	wallForce.y = player_node.child("movement").child("wallForce").attribute("y").as_float();

	fGravity = player_node.child("movement").child("gravity").text().as_float();

	//Limits
	fSlowGrade = player_node.child("movement").child("slowGrade").text().as_float();
	fSlowGradeAir = player_node.child("movement").child("slowGradeAir").text().as_float();
	fSlowGradeWall = player_node.child("movement").child("slowGradeWall").text().as_float();

	iSlowLimit = player_node.child("movement").child("slowLimit").text().as_int();

	fpPlayerMaxSpeed.x = player_node.child("movement").child("limitSpeed").attribute("x").as_float();
	fpPlayerMaxSpeed.y = player_node.child("movement").child("limitSpeed").attribute("y").as_float();


	//Create the player's collider
	SDL_Rect playerRect = { 0,0,0,0 };
	playerRect.w = player_node.child("collision").child("collider").attribute("w").as_float();
	playerRect.h = player_node.child("collision").child("collider").attribute("h").as_float();
	playerCollider = new Collider(playerRect, COLLIDER_PLAYER, this);

	//Internal variables load
	fInFramesLimit = player_node.child("internal").child("inFramesLimit").text().as_float();
	playerFadeTime = player_node.child("internal").child("playerFadeTime").text().as_float();
	wallJumpLimit = player_node.child("internal").child("wallJumpLimit").text().as_float();

	//Animation vars load
	pugi::xml_node animIterator = player_node.child("animations").child("animation");
	animIdle.loadAnimation(animIterator, "idle");
	animRun.loadAnimation(animIterator, "run");
	animWall.loadAnimation(animIterator, "wall");
	animJump.loadAnimation(animIterator, "jump");
	animFall.loadAnimation(animIterator, "fall");

	currentAnimation = &animIdle;


	//Sfx load
	jumpFx.path = player_node.child("fx").child("jump").attribute("path").as_string();
	deathFx.path = player_node.child("fx").child("death").attribute("path").as_string();
	landFx.path = player_node.child("fx").child("land").attribute("path").as_string();
	winFx.path = player_node.child("fx").child("win").attribute("path").as_string();
	bounceFx.path = player_node.child("fx").child("bounce").attribute("path").as_string();



	//Assign the value to the auxiliar node
	//We need this in order to load things later in start whose modules are not awoken yet
	auxLoader = player_node;

	//We start the time
	flCurrentTime = App->GetDeltaTime();

	return true;
}
// Called before the first frame
//We load things that need other modules to be awoken
bool j1Player::Start()
{
	//Load the player's textures
	playerTex = App->tex->Load(auxLoader.child("path").text().as_string());

	//Collision load
	App->collision->AddCollider(playerCollider);
	
	//Fx load
	jumpFx.id = App->audio->LoadFx(jumpFx.path.GetString());
	deathFx.id=App->audio->LoadFx(deathFx.path.GetString());
	landFx.id=App->audio->LoadFx(landFx.path.GetString());
	winFx.id=App->audio->LoadFx(winFx.path.GetString());
	bounceFx.id=App->audio->LoadFx(bounceFx.path.GetString());

	return true;
}
//Standard Movement
void j1Player::standardInputs()
{
	//Moving left
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{

		if (wallJumpDirection == DIRECTION_RIGHT)
			fpPlayerSpeed.x += (fpForce.x / fSlowGradeWall);
		else
			fpPlayerSpeed.x += fpForce.x;

		fpPlayerSpeed.x = deAccel(SLOW_NEGATIVE_LIMIT, fpPlayerSpeed.x, fSlowGrade, iSlowLimit);

		playerFlip = SDL_FLIP_HORIZONTAL;
	}
	//Moving right
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{

		if (wallJumpDirection == DIRECTION_LEFT)
			fpPlayerSpeed.x -= (fpForce.x / fSlowGradeWall);
		else
			fpPlayerSpeed.x -= fpForce.x;


		fpPlayerSpeed.x = deAccel(SLOW_POSITIVE_LIMIT, fpPlayerSpeed.x, fSlowGrade, iSlowLimit);

		playerFlip = SDL_FLIP_NONE;
	}
	else
	{
		//If neither key is pressed, we slow the player in both directions
		if (current_state == ST_GROUND)
			fpPlayerSpeed.x = deAccel(SLOW_GENERAL, fpPlayerSpeed.x, fSlowGrade);
		else
			fpPlayerSpeed.x = deAccel(SLOW_GENERAL, fpPlayerSpeed.x, fSlowGradeAir);

	}

	//Jump
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && (current_state == ST_GROUND || current_state == ST_WALL))
	{
		fPlayerAccel = 0; //Reset the accel
		fpPlayerSpeed.y = fpForce.y; //Add the jump force to the speed

		App->audio->PlayFx(jumpFx.id);

		inputs.add(IN_JUMP);
	}
	//Increasing speed to go down
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		fpPlayerSpeed.y -= fpForce.y;
	}



}

//Controls in God Mode movement
void j1Player::godInputs()
{
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		fpPlayerSpeed.x = -fpPlayerMaxSpeed.x;

	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		fpPlayerSpeed.x = fpPlayerMaxSpeed.x;

	}
	else
		fpPlayerSpeed.x = 0;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		fpPlayerSpeed.y = -fpPlayerMaxSpeed.y;

	}
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		fpPlayerSpeed.y = +fpPlayerMaxSpeed.y;

	}
	else
		fpPlayerSpeed.y = 0;



}

// Called each loop iteration
bool j1Player::PreUpdate()
{
	if (!god)
		standardInputs();
	else
		godInputs();
	
	//Get the time elapsed since the last frame
	flPreviousTime = flCurrentTime;
	flCurrentTime = App->GetDeltaTime();

	//The time gets corrected if it's too high
	if (flCurrentTime > fInFramesLimit)
		flCurrentTime = fInFramesLimit;

	//Check the player state and update to the next one
	UpdateState();

	return true;
}

//Updates the current state
void j1Player::UpdateState() 
{
	if(!god)
	{
	player_states state = process_fsm(inputs);
	current_state = state;
	}
}

// Called each loop iteration
bool j1Player::Update(float dt)
{
	//Logic for every playe state
	switch (current_state)
	{
	case ST_GROUND:
		LOG("GROUND\n");
		//The axis Y is 0
		fPlayerAccel = 0;
		fpPlayerSpeed.y = 0;
		//We check the player's speed and select his animation accordingly
		if ((fpPlayerSpeed.x > -fpForce.x) || (fpPlayerSpeed.x < fpForce.x))
		{
			currentAnimation = &animRun;
		}
		else
		{
			currentAnimation = &animIdle;
		}
		break;

	case ST_AIR:
		LOG("IN THE AIR ^^^^\n");
		fPlayerAccel += fGravity;

		//We check the player's speed and select his animation accordingly
		if (fpPlayerSpeed.y > 0.0f)
		{
		currentAnimation = &animJump;
		}
		else
		{
			currentAnimation = &animFall;
		}
		break;

	case ST_FALLING:
		LOG("FALLING \n");
		fPlayerAccel -= fpForce.y;
		currentAnimation = &animFall;
		break;

	case ST_WALL:
		LOG("WALLING \n");
		fPlayerAccel += fGravity/2.0f;
		currentAnimation = &animWall;
		break;

	case ST_WALL_JUMPING:
		LOG("WALL JUMPING \n");
		fPlayerAccel += fGravity;
		wallJumpTimer += flCurrentTime;
 		if (wallJumpTimer > wallJumpLimit) //When the player jumps, there's a limit in his speed to make him not stick to the wall for ever
		{
			wallJumpTimer = 0.0f;
			inputs.add(IN_JUMP);
			wallJumpDirection = DIRECTION_NONE;
		}
		currentAnimation = &animJump;
		break;

	case ST_GOD:
		LOG("GOD \n");
		falling = false;
		walling = false;
		currentAnimation = &animIdle;
		break;
	}

	//Limit & update position
	LimitPlayerSpeed();
	UpdatePos(flCurrentTime);
	
	return true;
}

void j1Player::UpdatePos(float dt)
{
	//If the logic does not demostrate the opposite, the player is always falling and not touching the wall
	falling = true;
	walling = false;

	//The update the player's position & speed according to it's logic
	fpPlayerPos.x += fpPlayerSpeed.x * dt;
	fpPlayerPos.y += fpPlayerSpeed.y * dt;
	if (current_state != ST_GROUND && current_state != ST_GOD)
	{
		fpPlayerSpeed.y += fPlayerAccel * dt;
	}

	//We set the collider in hte player's position
	CalculateCollider(fpPlayerPos);
}

// Limits the player speed in both axis
void j1Player::LimitPlayerSpeed()
{
	//X+
	if (fpPlayerSpeed.x > fpPlayerMaxSpeed.x)
	{
		fpPlayerSpeed.x = fpPlayerMaxSpeed.x;
	}

	//X-
	if (fpPlayerSpeed.x < -fpPlayerMaxSpeed.x)
	{
		fpPlayerSpeed.x = -fpPlayerMaxSpeed.x;
	}

	//Y+
	if (fpPlayerSpeed.y > fpPlayerMaxSpeed.y)
	{
		fpPlayerSpeed.y = fpPlayerMaxSpeed.y;
		fPlayerAccel = fPlayerAccel;
	}

	//Y-
	if (fpPlayerSpeed.y < -fpPlayerMaxSpeed.y)
	{
		fpPlayerSpeed.y = -fpPlayerMaxSpeed.y;
		fPlayerAccel = fPlayerAccel;
	}
}

//Sets the collider to the player's position
void j1Player::CalculateCollider(fPoint pos) 
{
	playerCollider->SetPos((int)fpPlayerPos.x, (int)fpPlayerPos.y);
}

//Distributes collisions according to it's type
void j1Player::OnCollision(Collider* playerCol, Collider* coll)
{
			switch (coll->type) {

			case(COLLIDER_WALL):
					RecalculatePos(playerCol->rect, coll->rect);
				break;
			case(COLLIDER_DEATH):
				App->fade->FadeToBlack(App->map->data.currentmap.GetString(), deathFx.id, playerFadeTime);
				break;
			case(COLLIDER_WIN):
				App->fade->FadeToBlack(App->map->GetNextMap(),winFx.id,playerFadeTime );				
				break;
			}
}

//Calculate the collisions with the enviroment
void j1Player::RecalculatePos(SDL_Rect playerRect, SDL_Rect collRect)
{
	//Determines the direction of the collision
	//Calculates distances from the player to the collision
	int collDiference[DIRECTION_MAX];
	collDiference[DIRECTION_LEFT] = (collRect.x + collRect.w) - playerRect.x;
	collDiference[DIRECTION_RIGHT] = (playerRect.x + playerRect.w) - collRect.x;
	collDiference[DIRECTION_UP] = (collRect.y + collRect.h) - playerRect.y;
	collDiference[DIRECTION_DOWN] = (playerRect.y + playerRect.h) - collRect.y;


	//If a collision from various aixs is detected, it determines what is the closets one to exit from
	int directionCheck = DIRECTION_NONE;

	for (int i = 0; i < DIRECTION_MAX; ++i)
	{
		if (directionCheck == DIRECTION_NONE)
				directionCheck = i;
		else if ((collDiference[i] < collDiference[directionCheck]))
				directionCheck = i;
	}

	//Then we update the player's position & logic according to it's movement & the minimum result that we just calculated
	switch (directionCheck) {
	case DIRECTION_UP:
		fpPlayerPos.y = collRect.y + collRect.h+1;
		fpPlayerSpeed.y = 0;
		break;
	case DIRECTION_DOWN:
		fpPlayerPos.y = collRect.y - playerRect.h ;
		fpPlayerSpeed.y = 0;
		fPlayerAccel = 0;
		falling = false;
		walling = false;
		inputs.add(IN_JUMP_FINISH);
		break;
	case DIRECTION_LEFT:
		fpPlayerPos.x = collRect.x + collRect.w;
		fpPlayerSpeed.x = 0;
		walling = true;
		falling = false;
		break;
	case DIRECTION_RIGHT:
		fpPlayerPos.x = collRect.x - playerRect.w;
 		fpPlayerSpeed.x = 0;
		walling = true;
		falling = false;
		break;
	case DIRECTION_NONE:
		break;
	}

	//We Recalculate the player's collider with the new position
	CalculateCollider(fpPlayerPos);
}

// Called each loop iteration
bool j1Player::PostUpdate()
{
	//If the player is not touching the ground, he is falling
	if (falling)
		inputs.add(IN_FALL);
	//Same if the player's on the wall
	if (walling)
		inputs.add(IN_WALL);

	//We set the flip accoording to the player's spped in x
	if (fpPlayerSpeed.x > 0)
		playerFlip = SDL_FLIP_NONE;
	else if (fpPlayerSpeed.x < 0)
		playerFlip = SDL_FLIP_HORIZONTAL;

	//And finally we blit
	App->render->Blit(playerTex, (int)fpPlayerPos.x, (int)fpPlayerPos.y, &currentAnimation->GetCurrentFrame(), 1.0f, playerFlip,0.0f, (currentAnimation->pivotpos->x), (currentAnimation->GetCurrentFrame().h / 2));

	return true;
}

// Called before quitting
bool j1Player::CleanUp()
{
	App->tex->UnLoad(playerTex);
	return true;
}

//Called when loading a save
bool j1Player::Load(pugi::xml_node& load)
{
	fpPlayerPos.x = load.child("position").attribute("x").as_float() - fSlowGrade;
	fpPlayerPos.y = load.child("position").attribute("y").as_float();
	fpPlayerSpeed.x = load.child("speed").attribute("x").as_float();
	fpPlayerSpeed.y = load.child("speed").attribute("y").as_float();
	
	falling = load.child("falling").attribute("value").as_bool();
	walling = load.child("walling").attribute("value").as_bool();
	wallJumpTimer = load.child("wallTimer").attribute("wallJumpTimer").as_float();

	switch (load.child("state").attribute("current_state").as_uint())
	{
	case 0:
		current_state = ST_UNKNOWN;
		break;
	case 1:
		current_state = ST_GROUND;
		break;
	case 2:
		current_state = ST_AIR;
		break;
	case 3:
		current_state = ST_FALLING;
		break;
	case 4:
		current_state = ST_WALL;
		break;
	case 5:
		current_state = ST_WALL_JUMPING;
		break;
	case 6:
		current_state = ST_GOD;
		break;
	}

	if (load.child("flip").attribute("value") == 0)
		playerFlip = SDL_FLIP_NONE;
	else
		playerFlip = SDL_FLIP_HORIZONTAL;

	return true;
}

//Called when loading a save
bool j1Player::Save(pugi::xml_node& save) const
{
	//Save all the player's status variables
	save.append_child("position").append_attribute("x") = fpPlayerPos.x;
	save.child("position").append_attribute("y") = fpPlayerPos.y;
	save.append_child("speed").append_attribute("x") = fpPlayerSpeed.x;
	save.child("speed").append_attribute("y") = fpPlayerSpeed.y;

	save.append_child("state").append_attribute("current_state") = current_state;
	save.append_child("falling").append_attribute("value") = falling;
	save.append_child("walling").append_attribute("value") = walling;
	save.append_child("wallTimer").append_attribute("wallJumpTimer") = wallJumpTimer;

	save.append_child("flip").append_attribute("value") = playerFlip;

	return true;
}

//Smoothly slows an speed axis
float j1Player::deAccel(slow_direction slow, float speedAxis, float grade, float limit)
{
	switch (slow) {

	case SLOW_GENERAL:
		speedAxis /= grade;
		break;

	case SLOW_AIR:
		speedAxis /= grade;
		break;

	case SLOW_LIMITS:
		if (speedAxis > limit)
			speedAxis /= grade;

		if (speedAxis < -limit)
			speedAxis /= grade;
		break;

	case SLOW_POSITIVE_LIMIT:
		if(speedAxis > limit)
			speedAxis /= grade;
		break;

	case SLOW_NEGATIVE_LIMIT:
		if(speedAxis < -limit)
			speedAxis /= grade;
		break;


	}
	return speedAxis;

}

//Changes the player's position
void j1Player::SetPos(int x, int y)
{
	fpPlayerPos.x = x;
	fpPlayerPos.y = y;
}

//Resets the player's movement
void j1Player::ReSetMovement()
{
	fpPlayerSpeed = { 0,0 };
	fPlayerAccel = 0;
	float wallJumpTimer = 0.0f;
}

//Controls god mode
void j1Player::GodMode()
{
	//God mode is turned on
	if (!god)
	{
		playerCollider->setType(COLLIDER_GOD);
		current_state = ST_GOD;
		fpPlayerSpeed.y = 0;
		fpPlayerSpeed.x = 0;
		god = true;
	}
	//God mode is turned off
	else if(god)
	{
		playerCollider->setType(COLLIDER_PLAYER);
		inputs.add(IN_FALL);
		god = false;
	}
}

//Iterates the states taking in account the inputs
player_states j1Player::process_fsm(p2List<player_inputs>& inputs)
{
	static player_states state = ST_GROUND;
	 player_inputs  last_input;

	while (inputs.Pop(last_input))
	{
		switch (state)
		{
		case ST_GROUND:
		{
			switch (last_input)
			{
		case IN_JUMP: state = ST_AIR;  break;
		case IN_FALL: state = ST_FALLING; break;
		case IN_GOD: state = ST_GOD; break;
			}
		}
		break;

		case ST_AIR:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: 
			{
				state = ST_GROUND;
				App->audio->PlayFx(landFx.id);
			}
				break;
			case IN_WALL: 
			{
				state = ST_WALL;
				InWall();
			} 
				break;

			case IN_GOD: state = ST_GOD; break;
			}
		}
		break;
		case ST_FALLING:
		{
			switch (last_input)
			{

			case IN_JUMP_FINISH:
			{
				state = ST_GROUND;
				App->audio->PlayFx(landFx.id);
			}
				break;

			case IN_GOD: state = ST_GOD; break;

			case IN_WALL: 
			{
				state =  ST_WALL;
				InWall();
			} 
			break;
			}
		}
		break;

		case ST_WALL:
		{
			switch (last_input)
			{
			case IN_FALL:
			{
				state = ST_FALLING;
			}
				break;
			case IN_JUMP: 
			{
				state = ST_WALL_JUMPING; 
				fpPlayerSpeed.y = wallForce.y; //The player's initial speed  when jumping off a wall
				fPlayerAccel = 0;
 				if (playerFlip == SDL_FLIP_NONE)
				{
					fpPlayerSpeed.x -= wallForce.x;
					wallJumpDirection = DIRECTION_LEFT;
				}
				else
				{
					fpPlayerSpeed.x += wallForce.x;
					wallJumpDirection = DIRECTION_RIGHT;
				}
			}  
				break;

			case IN_JUMP_FINISH:
			{
				state = ST_GROUND; 

			}
				break;
			case IN_GOD:
			{
				state = ST_GOD;
			}
				break;
			}
		}
		break;

		case ST_WALL_JUMPING:
		{
			switch (last_input)
			{
			case IN_JUMP: 
			{
				state = ST_AIR; 			
				wallJumpDirection = DIRECTION_NONE;
			} 
				break;

			case IN_JUMP_FINISH: 
			{
				state = ST_GROUND;
				wallJumpDirection = DIRECTION_NONE;
			}  
				break;

			case IN_WALL: 
			{
				state = ST_WALL;	
				InWall();
			}
				break;

			case IN_GOD: state = ST_GOD; break;

			}
		}
		break;

		case ST_GOD:
		{}
		break;
		}
	}
	return state;
}

//When the player's state turns wall
void j1Player::InWall()
{
	fPlayerAccel = 0;
	fpPlayerSpeed.y = deAccel(SLOW_GENERAL, fpPlayerSpeed.y, fSlowGradeWall);
	wallJumpDirection = DIRECTION_NONE;
	wallForceToMove = 0;
}
