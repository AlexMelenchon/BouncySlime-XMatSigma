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


	return true;
}

bool j1Player::Start()
{
	playerTex = App->tex->Load("textures/test.png");
	return true;
}

bool j1Player::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		playerPos.x -= playerSpeed.x;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		playerPos.y += playerSpeed.x;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		playerPos.y -= playerSpeed.x;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		playerPos.x += playerSpeed.x;
	}
	return true;
}

bool j1Player::Update(float dt)
{

	return true;
}

bool j1Player::PostUpdate()
{
	App->render->Blit(playerTex, (int)playerPos.x, (int)playerPos.y, idleRect);
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