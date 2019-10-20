#include "j1Player.h"
#include "j1Input.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1FadeToBlack.h"
#include "j1Map.h"


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

	idleRect = new SDL_Rect();
	//playerTex = App->tex->Load(player_node.child("path").attribute("value").as_string());	

	idleRect->x = player_node.child("idle").attribute("x").as_int();
	idleRect->y = player_node.child("idle").attribute("y").as_int();
	idleRect->w = 30;
	idleRect->h = 30;

	inputs.start = 0;


	playerCollider = new Collider(*idleRect, COLLIDER_PLAYER, this);
	App->collision->AddControlCollider(playerCollider);

	pugi::xml_node animIterator = player_node.child("animation");

	animIdle.PushBack(animIterator);
	animIterator = animIterator.next_sibling();
	animRun.PushBack(animIterator);
	animIterator = animIterator.next_sibling();
	animJump.PushBack(animIterator);
	animIterator = animIterator.next_sibling();
	animWall.PushBack(animIterator);
	animIterator = animIterator.next_sibling();
	animFall.PushBack(animIterator);
	animIterator = animIterator.next_sibling();



	currentAnimation = &animIdle;
	flCurrentTime = App->GetDeltaTime();

	return true;
}

bool j1Player::Start()
{
	playerTex = App->tex->Load("textures/player/idle.png");
	return true;
}

bool j1Player::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) 
	{

		if (wallJumpDirection == DIRECTION_RIGHT)
			fpPlayerSpeed.x += (fpForce.x / 20.0f);
		else
			fpPlayerSpeed.x += fpForce.x;

		fpPlayerSpeed.x = deAccel(SLOW_NEGATIVE_LIMIT, fpPlayerSpeed.x, slowGrade, slowLimit);

		playerFlip = SDL_FLIP_HORIZONTAL;
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{

		if (wallJumpDirection == DIRECTION_LEFT)
			fpPlayerSpeed.x -= fpForce.x / 20.0f;
		else
			fpPlayerSpeed.x -= fpForce.x;


		fpPlayerSpeed.x = deAccel(SLOW_POSITIVE_LIMIT, fpPlayerSpeed.x, slowGrade, slowLimit);

		playerFlip = SDL_FLIP_NONE;
	}
	else
	{
		if(current_state == ST_GROUND)
		fpPlayerSpeed.x = deAccel(SLOW_GENERAL, fpPlayerSpeed.x, slowGrade);
		else
		fpPlayerSpeed.x = deAccel(SLOW_GENERAL, fpPlayerSpeed.x, 1.05f);

	}


	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && (current_state == ST_GROUND || current_state == ST_WALL))
	{
		fPlayerAccel = 0; //Reset the accel
		fpPlayerSpeed.y = fpForce.y;


		inputs.add(IN_JUMP);
	}
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		fPlayerAccel -= fpForce.y;
	}


	//Get the time elapsed since the last frame
	flPreviousTime = flCurrentTime;
	flCurrentTime = App->GetDeltaTime();

	//The time gets corrected if it's too high
	if (flCurrentTime > 0.15f)
		flCurrentTime = 0.15f;

	//Check the player state and update to the next one
	UpdateState();


	return true;
}

void j1Player::UpdateState() 
{
	player_states state = process_fsm(inputs);
	current_state = state;
}

bool j1Player::Update(float dt)
{
	switch (current_state)
	{
	case ST_GROUND:
		LOG("IDLE\n");
		fPlayerAccel = 0;
		fpPlayerSpeed.y = 0;
		//current_animation = &idle;
		break;
	case ST_AIR:
		LOG("IN THE AIR ^^^^\n");
		fPlayerAccel += fGravity;
		//Mix_PlayChannel(-1, App->audio->effects[15], 0);
		break;
	case ST_FALLING:
		LOG("FALLING \n");
		fPlayerAccel -= fpForce.y;
		//Mix_PlayChannel(-1, App->audio->effects[15], 0);
		break;
	case ST_WALL:
		LOG("WALLING \n");
		fPlayerAccel += fGravity*2;
		//Mix_PlayChannel(-1, App->audio->effects[15], 0);
		break;
	case ST_WALL_JUMPING:
		LOG("WALL JUMPING \n");
		fPlayerAccel += fGravity;

		wallJumpTimer += flCurrentTime;
		if (wallJumpTimer > wallJumpLimit)
		{
			wallJumpTimer = 0.0f;
			inputs.add(IN_JUMP);
			wallJumpDirection = DIRECTION_NONE;
		}
		//Mix_PlayChannel(-1, App->audio->effects[15], 0);
		break;
	}

	//Update position
	LimitPlayerSpeed();
	UpdatePos(flCurrentTime);

	return true;
}

void j1Player::UpdatePos(float dt)
{
	falling = true;
	walling = false;

	fpPlayerPos.x += fpPlayerSpeed.x * dt;
	fpPlayerPos.y += fpPlayerSpeed.y * dt;

	if (current_state != ST_GROUND) {

		fpPlayerSpeed.y += fPlayerAccel * dt;
	}

	CalculateCollider(fpPlayerPos);

}

void j1Player::LimitPlayerSpeed()
{
	if (fpPlayerSpeed.x > fpPlayerMaxSpeed.x)
	{
		fpPlayerSpeed.x = fpPlayerMaxSpeed.x;
	}

	if (fpPlayerSpeed.x < -fpPlayerMaxSpeed.x)
	{
		fpPlayerSpeed.x = -fpPlayerMaxSpeed.x;
	}

	if (fpPlayerSpeed.y > fpPlayerMaxSpeed.y)
	{
		fpPlayerSpeed.y = fpPlayerMaxSpeed.y;
		fPlayerAccel = fPlayerAccel;
	}

	if (fpPlayerSpeed.y < -fpPlayerMaxSpeed.y)
	{
		fpPlayerSpeed.y = -fpPlayerMaxSpeed.y;
		fPlayerAccel = fPlayerAccel;
	}

}

void j1Player::CalculateCollider(fPoint pos) 
{
	playerCollider->ReSet((int)fpPlayerPos.x, (int)fpPlayerPos.y, currentAnimation->frames->w-15, currentAnimation->frames->h);

}

void j1Player::OnCollision(Collider* playerCol, Collider* coll)
{
	//Esto no deberían ser todos, sino que solo comprobara los que tiene cerca.

		switch (coll->type) {

		case(COLLIDER_WALL):
				RecalculatePos(playerCol->rect, coll->rect);
			break;
		case(COLLIDER_DEATH):
			if(control_death == false)
			App->fade->FadeToBlack(App->map->data.currentmap.GetString(), 0.6f);
			control_death = true;

			break;
		case(COLLIDER_WIN):
			if (control_death == false)
			App->fade->FadeToBlack(App->map->GetNextMap(), 0.6f);
			control_death = true;
			break;
		}


		//MORE CASES TO BE ADDED
}

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

	//Then we update the player's position & logic according to it's movement
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


bool j1Player::PostUpdate()
{
	//If the player is not touching the ground, he is falling
	if (falling)
		inputs.add(IN_FALL);
	//Same if the player's on the wall
	if (walling)
		inputs.add(IN_WALL);

	UpdateState();

	if (fpPlayerSpeed.x > 0)
		playerFlip = SDL_FLIP_NONE;
	else if (fpPlayerSpeed.x < 0)
		playerFlip = SDL_FLIP_HORIZONTAL;

	App->render->Blit(playerTex, (int)playerCollider->rect.x-10, (int)playerCollider->rect.y, &currentAnimation->GetCurrentFrame(), 1.0f, playerFlip,0.0f, 17,17);
	return true;
}

bool j1Player::CleanUp()
{
	App->tex->UnLoad(playerTex);
	return true;
}

bool j1Player::Load(pugi::xml_node&)
{
	return true;
}

bool j1Player::Save(pugi::xml_node&) const
{
	return true;
}

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

void j1Player::SetPos(int x, int y)
{
	fpPlayerPos.x = x;
	fpPlayerPos.y = y;
}

void j1Player::ReSetMovement()
{
	fpPlayerSpeed = { 0,0 };
	fPlayerAccel = 0;
	float wallJumpLimit = 0.3f;
	float wallJumpTimer = 0.0f;
}


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
			}
		}
		break;

		case ST_AIR:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: state = ST_GROUND; break;
			case IN_WALL: {state = ST_WALL;	fPlayerAccel = 0;} break;
			}
		}
		break;
		case ST_FALLING:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: state = ST_GROUND; break;
			case IN_WALL: 
			{
				state =  ST_WALL;	fPlayerAccel = 0; 
			} break;
			}
		}
		break;

		case ST_WALL:
		{
			switch (last_input)
			{
			case IN_FALL: state = ST_FALLING; break;
			case IN_JUMP: {
				state = ST_WALL_JUMPING; 
				fpPlayerSpeed.y = wallForce.y;
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
			}  break;
			case IN_JUMP_FINISH: state = ST_GROUND; break;
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
			} break;
			case IN_JUMP_FINISH: {
				state = ST_GROUND;
				wallJumpDirection = DIRECTION_NONE;
			}  break;
			case IN_WALL: 
			{
				state = ST_WALL;	
				fPlayerAccel = 0; 
				wallJumpDirection = DIRECTION_NONE;
			} break;
			}
		}
		break;


		}
	}
	return state;
}