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
#include "j1FlyingEnemy.h"
#include "j1Player.h"

//Constructor
j1FlyingEnemy::j1FlyingEnemy() : j1Enemy()
{
	this->type = entityType::FLYING_ENEMY;

}

//Destructor
j1FlyingEnemy ::~j1FlyingEnemy()
{
	path.Clear();

	if (collider != nullptr)
	{
		collider->to_delete = true;
		collider = nullptr;
	}
}


// Called before render is available
bool j1FlyingEnemy::Awake(pugi::xml_node& land_node)
{
	//Movement load
	chasingDistance = land_node.child("movement").child("chasingDistance").text().as_uint();
	chasingTiles = land_node.child("movement").child("chasingTiles").text().as_uint();
	idleSpeed.x = land_node.child("movement").child("idleSpeed").attribute("x").as_float();
	moveSpeed.x = land_node.child("movement").child("moveSpeed").attribute("x").as_float();
	idleSpeed.y = land_node.child("movement").child("idleSpeed").attribute("y").as_float();
	moveSpeed.y = land_node.child("movement").child("moveSpeed").attribute("y").as_float();
	fpMaxSpeed.x = land_node.child("movement").child("limitSpeed").attribute("x").as_float();
	fpMaxSpeed.y = land_node.child("movement").child("limitSpeed").attribute("y").as_float();


	// Internal variables load
	pathTimer = land_node.child("internal").child("pathTimer").text().as_float();
	scalesize = land_node.child("internal").child("scalesize").text().as_float();
	idleTimer = land_node.child("internal").child("idleTimer").text().as_float();
	chasingTimer = land_node.child("internal").child("chasingTimer").text().as_float();
	flipSpeed = land_node.child("internal").child("flipSpeed").text().as_float();


	//Create the player's collider
	SDL_Rect enemyRect = { 0,0,0,0 };
	enemyRect.w = land_node.child("collision").child("collider").attribute("w").as_float()*scalesize;
	enemyRect.h = land_node.child("collision").child("collider").attribute("h").as_int()*scalesize;

	collider = new Collider(enemyRect, COLLIDER_ENEMY, this);

	//Animation load
	pugi::xml_node animIterator = land_node.child("animations").child("animation");
	animIdle.loadAnimation(animIterator, "idle");
	currentAnimation = &animIdle;

	return true;
}

// Called before the first frame
bool j1FlyingEnemy::Start()
{
	//The enemy's texture load
	Text = App->entities->flying_tex;

	//Collision load
	App->collision->AddCollider(collider);

	return true;
}

// Called each loop iteration
bool j1FlyingEnemy::Update(float dt)
{
	timer += dt;

	bool ret = true;
	switch (state)
	{
	case enemy_state::ST_IDLE:
	{
		//Check if it's inside his patrol route
		if (collider->CheckCollision(trace))
		{
			//Affirmative: clears path & starts doing it's route
			path.Clear();
			TraceFollower();
		}
		else //Negative: searchs a path to it's route & moves towards it
		{
			//Search path timer
			if (timer > idleTimer)
			{
				GetPathfinding({float(trace.x +1), float(trace.y)}, false);
				timer = 0;
			}

			//Move if there's a path
			if (path.Count() > 0)
			{
				Move(dt);
			}

		}
		break;

		break;
	}
	case enemy_state::ST_CHASING:
	{
		//Search path timer
		if (timer > chasingTimer)
		{
			GetPathfinding({ App->entities->player->fpPosition.x, App->entities->player->fpPosition.y + App->entities->player->collider->rect.w / 1.5f }, true);
			timer = 0;
		}

		//Move if there's a path
		if (path.Count() > 0)
		{
			//The update the enemy's position & speed according to it's logic
			Move(dt);
		}
		break;
	}

	case enemy_state::ST_UNKNOWN:
	{
		ret = false;
		break;
	}
	}
	UpdatePos(dt);
	
	return ret;
}

//Moves the enemy according to pathfinding
void j1FlyingEnemy::Move(float dt)
{
	//Get the next tile to move
	iPoint current = App->map->MapToWorld(path.At(path.Count() - 1)->x, path.At(path.Count() - 1)->y);

	//If the tile is a tile of distance of the enemy, it moves
	if (abs(abs(fpPosition.x) - abs(current.x)) > App->map->data.tile_height || abs(abs(fpPosition.y) - abs(current.y)) > App->map->data.tile_height)
	{
		if (fpPosition.x < current.x)
		{
			fpSpeed.x += moveSpeed.x * (dt * VEL_TO_WORLD);
			if (fpSpeed.x < 0)
				fpSpeed.x = 0;
		}
		else if (fpPosition.x > current.x)
		{
			fpSpeed.x -= moveSpeed.x * (dt * VEL_TO_WORLD);
			if (fpSpeed.x > 0)
				fpSpeed.x = 0;
		}

		if (fpPosition.y < current.y)
		{
			fpSpeed.y += moveSpeed.y * (dt * VEL_TO_WORLD);
			if (fpSpeed.y < 0)
				fpSpeed.y = 0;
		}

		else if (fpPosition.y > current.y)
		{
			fpSpeed.y -= moveSpeed.y * (dt * VEL_TO_WORLD);
			if (fpSpeed.y > 0)
				fpSpeed.y = 0;
		}

	}
	else
		path.Pop(current);
}

//When the enemy is idle, it has a defined movement
void j1FlyingEnemy::TraceFollower()
{

	switch (traceDir)
	{
	case (DIRECTION_RIGHT):

		fpSpeed.y = 0;
		fpSpeed.x = idleSpeed.x;

		if (fpPosition.x >= trace.x + trace.w - collider->rect.w)
		{
			traceDir = DIRECTION_DOWN;
		}

		break;

	case (DIRECTION_DOWN):

		fpSpeed.x = 0;
		fpSpeed.y = idleSpeed.y;
		if (fpPosition.y > trace.y + trace.h - collider->rect.h)
		{
			traceDir = DIRECTION_LEFT;

		}

		break;

	case (DIRECTION_LEFT):

		fpSpeed.y = 0;
		fpSpeed.x = -idleSpeed.x;

		if (fpPosition.x <= trace.x)
		{
			traceDir = DIRECTION_UP;

		}

		break;

	case (DIRECTION_UP):
		fpSpeed.x = 0;
		fpSpeed.y = -idleSpeed.y;
		if (fpPosition.y < trace.y)
		{
			traceDir = DIRECTION_RIGHT;

		}
		break;

	}
}

//Update entity's position
void j1FlyingEnemy::UpdatePos(float dt)
{
	//If the logic does not demostrate tshe opposite, the player is always falling and not touching the wall
	falling = true;

	//Limit Speed
	LimitSpeed();

	fpPosition.x += fpSpeed.x * dt;
	fpPosition.y += fpSpeed.y * dt;

	//We set the collider in hte player's position
	CalculateCollider(fpPosition);
}

//If a collision is detected by the j1Collision, distributes collisions according to it's type
void j1FlyingEnemy::OnCollision(Collider* entityCol, Collider* coll)
{
	switch (coll->type) {

	case(COLLIDER_WALL):
		RecalculatePos(entityCol->rect, coll->rect);
		break;

	case(COLLIDER_DEATH):
		entityCol->to_delete = true;
		break;
	}
}

//Calculate the collisions with the enviroment
void j1FlyingEnemy::RecalculatePos(SDL_Rect entityColl, SDL_Rect collRect)
{
	//If a collision from various aixs is detected, it determines what is the closets one to exit from
	int directionCheck = CheckCollisionDir(entityColl, collRect);

	//Then we update the player's position & logic according to it's movement & the minimum result that we just calculated
	switch (directionCheck) {
	case DIRECTION_UP:
		fpPosition.y = collRect.y + collRect.h + 2;
		break;
	case DIRECTION_DOWN:
		fpPosition.y = collRect.y - entityColl.h;
		break;
	case DIRECTION_LEFT:
		fpPosition.x = collRect.x + collRect.w;
		break;
	case DIRECTION_RIGHT:
		fpPosition.x = collRect.x - entityColl.w;
		break;
	case DIRECTION_NONE:
		break;
	}
	//We Recalculate the entity's collider with the new position

	CalculateCollider(fpPosition);

}

//Called when loading a save
bool j1FlyingEnemy::Load(pugi::xml_node& load)
{

	fpPosition.x = load.child("position").attribute("x").as_float();
	fpPosition.y = load.child("position").attribute("y").as_float();
	fpSpeed.x = load.child("speed").attribute("x").as_float();
	fpSpeed.y = load.child("speed").attribute("y").as_float();

	falling = load.child("falling").attribute("value").as_bool();

	//TODO, save pathfinding timer
	//save.append_child("wallTimer").append_attribute("wallJumpTimer") = wallJumpTimer;
	//wallJumpTimer = load.child("wallTimer").attribute("wallJumpTimer").as_float();

	if (load.child("flip").attribute("value") == 0)
		Flip = SDL_FLIP_NONE;
	else
		Flip = SDL_FLIP_HORIZONTAL;

	return true;
}

//Called when loading a save
bool j1FlyingEnemy::Save(pugi::xml_node& save) const
{
	pugi::xml_node eneNode;

	eneNode = save.append_child("enemyF");

	//Save all the player's status variables
	eneNode.append_child("position").append_attribute("x") = fpPosition.x;
	eneNode.child("position").append_attribute("y") = fpPosition.y;
	eneNode.append_child("speed").append_attribute("x") = fpSpeed.x;
	eneNode.child("speed").append_attribute("y") = fpSpeed.y;

	eneNode.append_child("falling").append_attribute("value") = falling;

	//TODO, save pathfinding timer
	//save.append_child("wallTimer").append_attribute("wallJumpTimer") = wallJumpTimer;

	eneNode.append_child("flip").append_attribute("value") = Flip;

	return true;
}