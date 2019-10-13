#include "j1Player.h"
#include "j1Input.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collision.h"
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
	idleRect->w = 35;
	idleRect->h = 35;

	inputs.start = 0;


	playerCollider = new Collider(*idleRect, COLLIDER_PLAYER, this);
	App->collision->AddControlCollider(playerCollider);

	animPlayerIdle = new Animation();
	for (pugi::xml_node iterator = player_node.child("sprite"); iterator; iterator = iterator.next_sibling("sprite"))
	{
		SDL_Rect frame;
		frame.x = iterator.attribute("x").as_int();
		frame.y = iterator.attribute("y").as_int();
		frame.w = iterator.attribute("w").as_int();
		frame.h = iterator.attribute("h").as_int();
		animPlayerIdle->PushBack(frame, iterator.attribute("frames").as_int());
	}
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
		fpPlayerSpeed.x += fpForce.x;
		deAccel(SLOW_NEGATIVE_X);

		playerFlip = SDL_FLIP_HORIZONTAL;
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) 
	{
		fpPlayerSpeed.x -= fpForce.x;
		deAccel(SLOW_POSITIVE_X);

		playerFlip = SDL_FLIP_NONE;
	}
	else
	{
		if(current_state == ST_GROUND)
		deAccel(SLOW_GENERAL);
		else
		deAccel(SLOW_AIR);

	}


	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && (current_state == ST_GROUND || current_state == ST_WALL))
	{
		fPlayerAccel = 0; //Reset the accel
		fpPlayerSpeed.y = fpForce.y;


		inputs.add(IN_JUMP);
	}
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		fPlayerAccel -= -fpForce.y;
	}


	//Get the time elapsed since the last frame
	flPreviousTime = flCurrentTime;
	flCurrentTime = App->GetDeltaTime();

	//The time gets corrected if it's too high
	if (flCurrentTime > 0.15)
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
	checkCollision(playerCollider);

	//If the player is not touching the ground, he is falling
	if (falling)
		inputs.add(IN_FALL);
	//Same if the player's on the wall
	if (walling)
		inputs.add(IN_WALL);
	
	UpdateState();
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
	playerCollider->ReSet((int)fpPlayerPos.x, (int)fpPlayerPos.y, animPlayerIdle->frames->w, animPlayerIdle->frames->h);

}

void j1Player::checkCollision(Collider* playerCol)
{
	//Esto no deberían ser todos, sino que solo comprobara los que tiene cerca.
	p2List_item<Collider*>* colliderList = App->map->data.colliderList.start;
	for (colliderList; colliderList; colliderList = colliderList->next)
	{
		switch (colliderList->data->type) {

		case(COLLIDER_WALL):

			if (playerCol->CheckCollision(colliderList->data->rect))
			{
				RecalculatePos(playerCol->rect, colliderList->data->rect);
			}
			break;
		}

		//MORE CASES TO BE ADDED
		//CHANGE TO A SWITCH
	}

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

	bool collDirection[DIRECTION_MAX];
	collDirection[DIRECTION_RIGHT] = false;
	collDirection[DIRECTION_LEFT] = false;
	collDirection[DIRECTION_UP] = false;
	collDirection[DIRECTION_DOWN] = false;

	collDirection[DIRECTION_RIGHT] = !(collDiference[DIRECTION_RIGHT] > 0 && fpPlayerSpeed .x< 0);
	collDirection[DIRECTION_LEFT] = !(collDiference[DIRECTION_RIGHT] > 0 && fpPlayerSpeed.x > 0);
	collDirection[DIRECTION_UP] = !(collDiference[DIRECTION_UP] > 0 && fpPlayerSpeed.y < 0);
	collDirection[DIRECTION_DOWN] = !(collDiference[DIRECTION_DOWN] >= 0 && fpPlayerSpeed.y > 0);



	//If a collision from various aixs is detected, it determines what is the closets one to exit from
	int directionCheck = DIRECTION_NONE;

	for (int i = 0; i < DIRECTION_MAX; ++i)
	{
		if (collDirection[i] && directionCheck == DIRECTION_NONE)
				directionCheck = i;
		else if (collDiference[i] < collDiference[directionCheck])
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
	if (fpPlayerSpeed.x > 0)
		playerFlip = SDL_FLIP_NONE;
	else if (fpPlayerSpeed.x < 0)
		playerFlip = SDL_FLIP_HORIZONTAL;

	App->render->Blit(playerTex, (int)playerCollider->rect.x, (int)playerCollider->rect.y, &animPlayerIdle->GetCurrentFrame(), 1.0f, playerFlip);
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

void j1Player::deAccel(slow_direction slow)
{
	switch (slow) {

	case SLOW_GENERAL:
		fpPlayerSpeed.x /= slowGrade;
		break;

	case SLOW_AIR:
		fpPlayerSpeed.x /= 1.05f;
		break;

	case SLOW_POSITIVE_X:
		if(fpPlayerSpeed.x > slowLimit)
		fpPlayerSpeed.x /= slowGrade;
		break;

	case SLOW_NEGATIVE_X:
		if(fpPlayerSpeed.x < -slowLimit)
		fpPlayerSpeed.x /= slowGrade;
		break;
	}

}

void j1Player::SetPos(int x, int y)
{
	fpPlayerPos.x = x;
	fpPlayerPos.y = y;
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
			case IN_WALL: state: {state = ST_WALL;	fPlayerAccel = 0;} break;
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
			case IN_JUMP: 
			{ 
 				state = ST_AIR;
				fpPlayerSpeed.y = wallForce.y;
				fPlayerAccel = 0;
				if(playerFlip == SDL_FLIP_NONE)
 				fpPlayerSpeed.x -= wallForce.x;
				else
				fpPlayerSpeed.x += wallForce.x ;
			
			}; break;
			case IN_JUMP_FINISH: state = ST_GROUND; break;
			}
		}
		break;


		}
	}
	return state;
}