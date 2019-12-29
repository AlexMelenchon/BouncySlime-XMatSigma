#include "j1Coin.h"
#include "j1Collision.h"
#include "j1EntityManager.h"
#include "j1Audio.h"
#include "j1Scene.h"


//Constructor
j1Coin::j1Coin() : j1Entity()
{
	this->type = entityType::COIN;
}

//Destructor
j1Coin ::~j1Coin()
{}


// Called before render is available
bool j1Coin::Awake(pugi::xml_node& coin_node)
{
	//Create the coin's collider
	scalesize = coin_node.child("collision").child("scalesize").text().as_uint();

	SDL_Rect particleRect = { 0,0,0,0 };
	particleRect.w = coin_node.child("collision").child("collider").attribute("w").as_float() * scalesize;
	particleRect.h = coin_node.child("collision").child("collider").attribute("h").as_float() * scalesize;

	//collider load
	collider = new Collider(particleRect, COLLIDER_COIN, this);

	//animation load
	pugi::xml_node animIterator = coin_node.child("animations").child("animation");
	anim.loadAnimation(animIterator, "coin");

	currentAnimation = &anim;

	return true;
}

// Called before the first frame
bool j1Coin::Start()
{
	//The enemy's texture load
	Text = App->entities->player_tex;

	//Collision load
	App->collision->AddCollider(collider);

	return true;
}

// Called each loop iteration
bool j1Coin::Update(float dt)
{
	bool ret = true;

	//Calculate the coin collider posttion
	CalculateCollider(this->fpPosition);

	return ret;
}

// Called each loop iteration
bool j1Coin::PostUpdate(bool debug)
{
	Draw();
	return true;
}

// Called before quitting
bool j1Coin::CleanUp()
{
	if (collider != nullptr)
	{
		collider->to_delete = true;
		collider = nullptr;
	}
	Text = nullptr;

	return true;
}


// Called each loop iteration
void j1Coin::OnCollision(Collider* coinCol, Collider* coll)
{
	switch (coll->type)
	{
	case(COLLIDER_PLAYER):
		coinCol->to_delete = true;
		break;

	case(COLLIDER_GOD):
		if(!coinCol->to_delete)
			App->scene->CoinUp();

		coinCol->to_delete = true;
		break;
	}
}


//Called when loading a save
bool j1Coin::Load(pugi::xml_node& load)
{
	fpPosition.x = load.child("position").attribute("x").as_float();
	fpPosition.y = load.child("position").attribute("y").as_float();

	return true;
}

//Called when loading a save
bool j1Coin::Save(pugi::xml_node& save) const
{
	pugi::xml_node coinNode;
	coinNode = save.append_child("coin");

	//Save all the player's status variables
	coinNode.append_child("position").append_attribute("x") = fpPosition.x;
	coinNode.child("position").append_attribute("y") = fpPosition.y;

	return true;
}