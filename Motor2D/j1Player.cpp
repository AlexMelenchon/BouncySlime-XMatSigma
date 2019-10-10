#include "j1Player.h"
#include "j1Input.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"


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
	idleRect->w = 500;
	idleRect->h = 300;

	inputs.start = 0;


	flCurrentTime = App->GetDeltaTime();

	return true;
}

bool j1Player::Start()
{
	playerTex = App->tex->Load("textures/test.png");
	return true;
}

bool j1Player::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) 
	{
		fpPlayerSpeed.x += fpForce.x;
		deAccel(SLOW_NEGATIVE_X);

		playerFlip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) 
	{
		fpPlayerSpeed.x -= fpForce.x;
		deAccel(SLOW_POSITIVE_X);

		playerFlip = SDL_RendererFlip::SDL_FLIP_NONE;
	}
	else
	{
		deAccel(SLOW_GENERAL);
	}


	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && current_state != ST_AIR)
	{
		fpPlayerSpeed.y = fpForce.y;

		inputs.add(IN_JUMP);
	}
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		fpPlayerAccel.y -= -fpForce.y;
	}


	//Get the time elapsed since the last frame
	flPreviousTime = flCurrentTime;
	flCurrentTime = App->GetDeltaTime();

	//Update position
	LimitPlayerSpeed();
	UpdatePos(flCurrentTime);

	//Check the player state and upload the new one
	player_states state = process_fsm(inputs);
	current_state = state;

	if (fpPlayerPos.y > 200) inputs.add(IN_JUMP_FINISH);

	return true;
}

void j1Player::UpdatePos(float dt)
{
	fpPlayerPos.x += fpPlayerSpeed.x * dt;

	fpPlayerPos.y += fpPlayerSpeed.y * dt;
	fpPlayerSpeed.y += fpPlayerAccel.y * dt;

	//TODO: UPDATE AND CHECK COLLISION, IF FALSE, WE CONSIDER THE PLAYER IS FALLING
	//	inputs.add(IN_JUMP_FINISH);
}

void j1Player::LimitPlayerSpeed() 
{
	if (fpPlayerSpeed.x > fpPlayerMaxSpeed.x)
	{
		fpPlayerSpeed.x = fpPlayerMaxSpeed.x;
		fpPlayerAccel.x = fpPlayerAccel.x;
	}

	if (fpPlayerSpeed.x < -fpPlayerMaxSpeed.x)
	{
		fpPlayerSpeed.x = -fpPlayerMaxSpeed.x;
		fpPlayerAccel.x = fpPlayerAccel.x;
	}

	if (fpPlayerSpeed.y > fpPlayerMaxSpeed.y)
	{
		fpPlayerSpeed.y = fpPlayerMaxSpeed.y;
		fpPlayerAccel.y = fpPlayerAccel.y;
	}

	if (fpPlayerSpeed.y < -fpPlayerMaxSpeed.y)
	{
		fpPlayerSpeed.y = -fpPlayerMaxSpeed.y;
		fpPlayerAccel.y = fpPlayerAccel.y;
	}

}

bool j1Player::Update(float dt)
{
	switch (current_state)
	{
	case ST_GROUND:
		LOG("IDLE\n");
		fpPlayerAccel.y = 0;
		fpPlayerSpeed.y = 0;
		//current_animation = &idle;
		break;
		break;
	case ST_AIR:
		LOG("IN THE AIR ^^^^\n");
		fpPlayerAccel.y += -fGravity;
		//Mix_PlayChannel(-1, App->audio->effects[15], 0);
		break;

	}
	return true;
}

bool j1Player::PostUpdate()
{
	App->render->Blit(playerTex, (int)fpPlayerPos.x, (int)fpPlayerPos.y, idleRect, 1.0f, playerFlip);
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

			}
		}
		break;

		case ST_AIR:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: state = ST_GROUND; break;

			}
		}
		break;


		}
	}
	return state;


}