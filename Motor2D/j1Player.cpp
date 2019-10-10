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
		fpPlayerAccel.x += fpForce.x;

		playerFlip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) 
	{
		fpPlayerAccel.x -= fpForce.x;

		playerFlip = SDL_RendererFlip::SDL_FLIP_NONE;
	}
	else
	{

		if (fpPlayerSpeed.x > 10) 
		{
			fpPlayerAccel.x = 0;
			fpPlayerSpeed.x += fpForce.x;
		}
		else if (fpPlayerSpeed.x < -10)
		{
			fpPlayerAccel.x = 0;
			fpPlayerSpeed.x -= fpForce.x;
		}
		else {
			fpPlayerSpeed.x = 0.0f;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		fpPlayerPos.y += fpPlayerSpeed.x;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		inputs.add(IN_JUMP);
	}

	//Get the time elapsed since the last frame
	flPreviousTime = flCurrentTime;
	flCurrentTime = App->GetDeltaTime();

	//Update position
	LimitPlayerSpeed();
	UpdatePos(flCurrentTime);

	//Check the player state and upload the new one
	player_states current_state = process_fsm(inputs);



	return true;
}

void j1Player::UpdatePos(float dt)
{
	fpPlayerPos.x += fpPlayerSpeed.x * dt;
	fpPlayerSpeed.x += fpPlayerAccel.x * dt;

}

void j1Player::LimitPlayerSpeed()
{
	if (fpPlayerSpeed.x > fpPlayerMaxSpeed.x)
	{
		fpPlayerSpeed.x = fpPlayerMaxSpeed.x;
		fpPlayerAccel.x = 0;
	}

}

bool j1Player::Update(float dt)
{
	switch (current_state)
	{
	case ST_IDLE:
		LOG("IDLE\n");
		//current_animation = &idle;
		break;
	case ST_WALK_FORWARD:
		LOG("FORWARD >>>\n");
		//iplayerPos.x += iplayerSpeed.x;

		break;
	case ST_WALK_BACKWARD:
		LOG("BACKWARD <<<\n");
		//iplayerPos.y -= iplayerSpeed.x;

		break;
	case ST_JUMP:
		LOG("JUMPING NEUTRAL ^^^^\n");
		//Mix_PlayChannel(-1, App->audio->effects[15], 0);
		break;
	case ST_JUMP_FORWARD:
		LOG("JUMPING FORWARD ^^>>\n");
		//Mix_PlayChannel(-1, App->audio->effects[15], 0);
		break;
	case ST_JUMP_BACKWARD:
		LOG("JUMPING BACKWARD ^^<<\n");

		//Mix_PlayChannel(-1, App->audio->effects[15], 0);
		break;
	}
	return true;
}

bool j1Player::PostUpdate()
{
	App->render->Blit(playerTex, (int)fpPlayerPos.x, (int)fpPlayerPos.y, idleRect);
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


player_states j1Player::process_fsm(p2List<player_inputs>& inputs)
{
	static player_states state = ST_IDLE;
	 player_inputs  last_input;

	while (inputs.Pop(last_input))
	{
		switch (state)
		{
		case ST_IDLE:
		{
			switch (last_input)
			{
			case IN_RIGHT: state = ST_WALK_FORWARD; break;
			case IN_LEFT: state = ST_WALK_BACKWARD; break;
			case IN_JUMP: state = ST_JUMP;  break;

			}
		}
		break;

		case ST_JUMP:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH: state = ST_IDLE; break;

			}
		}
		break;


		}
	}
	return state;


}