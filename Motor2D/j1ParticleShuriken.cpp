#include "j1LandEnemy.h"
#include "j1Player.h"
#include "j1Input.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1FadeToBlack.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "j1EntityManager.h"
#include "j1Entity.h"
#include "j1Pathfinding.h"
#include "j1Scene.h"
#include "j1ParticleShuriken.h"


j1ParticleShuriken::j1ParticleShuriken() : j1Entity()
{
	this->type = entityType::SHURIKEN;
}

j1ParticleShuriken ::~j1ParticleShuriken()
{}


bool j1ParticleShuriken::Awake(pugi::xml_node& shuriken_node)
{
	
	//Create the player's collider
	SDL_Rect particleRect = { 0,0,0,0 };
	particleRect.w = shuriken_node.child("collision").child("collider").attribute("w").as_float();
	particleRect.h = shuriken_node.child("collision").child("collider").attribute("h").as_float();

	collider = new Collider(particleRect, COLLIDER_SHURIKEN, this);	

	pugi::xml_node animIterator = shuriken_node.child("animations").child("animation");
	anim.loadAnimation(animIterator, "shuriken");	
	
	currentAnimation = &anim;

	auxLoader = shuriken_node;
	return true;
}

bool j1ParticleShuriken::Start()
{
	//The enemy's texture load
	Text = App->entities->player->Text;

	//Collision load
	App->collision->AddCollider(collider);

	CheckDir();

	return true;
}

bool j1ParticleShuriken::PreUpdate()
{
	UpdateState();

	return true;
}

bool j1ParticleShuriken::Update(float dt)
{
	bool ret = true;
	UpdatePos(dt);

	return ret;
}



bool j1ParticleShuriken::PostUpdate(bool debug)
{
	Draw();	

	return true;
}

bool j1ParticleShuriken::CleanUp()
{
	if (collider != nullptr)
	{
		collider->to_delete = true;
		collider = nullptr;
	}
	
	return true;
}

void j1ParticleShuriken::UpdatePos(float dt)
{
	//If the logic does not demostrate tshe opposite, the player is always falling and not touching the wall
	falling = true;

	//Limit Speed
	//LimitSpeed(dt);

	fpPosition.x += fpSpeed.x * dt;
	fpPosition.y += fpSpeed.y * dt;



	//We set the collider in hte player's position
	CalculateCollider(fpPosition);
}

void j1ParticleShuriken::CheckDir()
{
	//X-
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		fpSpeed.x = -1000;
	}
	//X+
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		fpSpeed.x = 1000;
	}

	//Y+
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		fpSpeed.y = -1000;
	}
	//Y-
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		fpSpeed.y = 1000;
	}
}


void j1ParticleShuriken::OnCollision(Collider* entityCol, Collider* coll)
{
	switch (coll->type) {

	case(COLLIDER_WALL):
		RecalculatePos(entityCol->rect, coll->rect);
		break;

	case(COLLIDER_ENEMY):
		coll->to_delete = true;
		break;
	}
}

void j1ParticleShuriken::RecalculatePos(SDL_Rect entityColl, SDL_Rect collRect)
{
	//Determines the direction of the collision
	//Calculates distances from the player to the collision
	int collDiference[DIRECTION_MAX];
	collDiference[DIRECTION_LEFT] = (collRect.x + collRect.w) - entityColl.x;
	collDiference[DIRECTION_RIGHT] = (entityColl.x + entityColl.w) - collRect.x;
	collDiference[DIRECTION_UP] = (collRect.y + collRect.h) - entityColl.y;
	collDiference[DIRECTION_DOWN] = (entityColl.y + entityColl.h) - collRect.y;


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
		fpPosition.y = collRect.y + collRect.h + 2;
		fpSpeed.y = 0;
		break;
	case DIRECTION_DOWN:
		fpPosition.y = collRect.y - entityColl.h;
		fpSpeed.y = 0;
		fAccel = 0;
		falling = false;

		break;
	case DIRECTION_LEFT:
		fpPosition.x = collRect.x + collRect.w;
		if (fpSpeed.x < 0)
			fpSpeed.x = 0;
		falling = false;
		break;
	case DIRECTION_RIGHT:
		fpPosition.x = collRect.x - entityColl.w;
		if (fpSpeed.x > 0)
			fpSpeed.x = 0;
		falling = false;
		break;
	case DIRECTION_NONE:
		break;
	}
	//We Recalculate the entity's collider with the new position

	CalculateCollider(fpPosition);

}


