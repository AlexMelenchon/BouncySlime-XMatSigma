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

//Constructor
j1LandEnemy::j1LandEnemy() : j1Enemy()
{	
	this->type = entityType::LAND_ENEMY;
	
}

//Destructor
j1LandEnemy ::~j1LandEnemy()
{
	path.Clear();

	path.~p2DynArray();

	if (collider != nullptr)
	{
		collider->to_delete = true;
		collider = nullptr;
	}
}

bool j1LandEnemy::Awake(pugi::xml_node& land_node)
{
	//Movement load
	chasingDistance = land_node.child("movement").child("chasingDistance").text().as_uint();
	chasingTiles = land_node.child("movement").child("chasingTiles").text().as_uint();
	jumpForce = land_node.child("movement").child("jumpForce").text().as_uint();
	jumpDistance = land_node.child("movement").child("jumpDistance").text().as_uint();
	idleSpeed.x = land_node.child("movement").child("idleSpeed").text().as_uint();
	moveSpeed.x = land_node.child("movement").child("moveSpeed").text().as_uint();
	fallingSpeed = land_node.child("movement").child("fallingSpeed").text().as_uint();
	fpMaxSpeed.x = land_node.child("movement").child("limitSpeed").attribute("x").as_float();
	fpMaxSpeed.y = land_node.child("movement").child("limitSpeed").attribute("y").as_float();
	


	//Internal variables load
	pathTimer = land_node.child("internal").child("pathTimer").text().as_float();
	scalesize = land_node.child("internal").child("scalesize").text().as_float();
	idleTimer = land_node.child("internal").child("idleTimer").text().as_float();
	chasingTimer = land_node.child("internal").child("chasingTimer").text().as_float();
	flipSpeed = land_node.child("internal").child("flipSpeed").text().as_float();

	//Create the player's collider
	SDL_Rect enemyRect = { 0,0,0,0 };
	enemyRect.w = land_node.child("collision").child("collider").attribute("w").as_float() * scalesize;
	enemyRect.h = land_node.child("collision").child("collider").attribute("h").as_float() * scalesize;

	collider = new Collider(enemyRect, COLLIDER_ENEMY, this);

	pugi::xml_node animIterator = land_node.child("animations").child("animation");
	animIdle.loadAnimation(animIterator, "idle");
	animRun.loadAnimation(animIterator, "run");
	

	currentAnimation = &animIdle;

	return true;
}

// Called before the first frame
bool j1LandEnemy::Start()
{
	//The enemy's texture load
	Text = App->entities->land_tex;

	//Collision load
	App->collision->AddCollider(collider);

	return true;
}

// Called each loop iteration
bool j1LandEnemy::Update(float dt)
{
	BROFILER_CATEGORY("Enemy Update", Profiler::Color::Lavender)
	timer += dt;

	bool ret = true;
	switch (state)
	{
	case enemy_state::ST_IDLE:
		{
		//Check if it's inside his patrol route
		currentAnimation = &animIdle;
		
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
				GetPathfinding({float(trace.x+1), float(trace.y)}, false);
				timer = 0;
			}
			
			//Path is created
			if (path.Count() > 0)
			{
				MovementLogic(dt, false);
			}
			//If there is no path we stop
			else
				fpSpeed.x = 0;

		}		
		break;
		}
	case enemy_state::ST_CHASING:
		{
		currentAnimation = &animRun;

		//Search path timer
		if (timer > chasingTimer && !falling)
		{
			GetPathfinding({ App->entities->player->fpPosition.x, App->entities->player->fpPosition.y + App->entities->player->collider->rect.w / 1.5f }, true);
			timer = 0;
		}
		
		//Path is created
		if (path.Count() > 0)
		{
			MovementLogic(dt, true);
		}
		//If there is no path we stop
		else
			fpSpeed.x = 0;

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

//Updates the enemy movement logic
void j1LandEnemy::MovementLogic(float dt, bool toPlayer)
{
	//We check if the enemy has to jump to get to the player
	//If the logic says we have to jump we jump & move
	if (!JumpLogic())
	{
		Move(toPlayer, dt);
	}
	//If the logic says we have to stop, we check if we can move some tiles before stopping
	// (This is not an or in the previous if for visability purposes)
	else if (AbleToMove().x != -1)
	{
		Move(toPlayer, dt);
	}
	//If  the logic says we have to stop, we stop
	else
		fpSpeed.x = 0;

}

//Gets if the next tile, in the enemy's foot is walkable
iPoint j1LandEnemy::NextWalkableTile()
{
		iPoint ret = { -1, -1 };

		//We check all the tiles
		iPoint next = { path.At(path.Count() - 1)->x, path.At(path.Count() - 1)->y + 2 };
		if (App->pathfinding->IsWalkable(next) && !falling)
		{
			for (uint i = path.Count() -1; i > 0; --i)
			{
				next = { path.At(i)->x, path.At(i)->y + 2 };
				//If one tile is not walkable, which means is air, we return it
				if (!App->pathfinding->IsWalkable(next))
				{
					ret = next;
					break;
				}
			}
		}
		return ret;
}

//Checks if he cant move to the next tile without falling
iPoint j1LandEnemy::AbleToMove()
{
	iPoint ret = { -1, -1 };

	//We check all the tiles
	iPoint next = { path.At(path.Count() - 1)->x, App->map->WorldToMap(fpPosition.x, (int)round(fpPosition.y)).y +3 };
	if (!App->pathfinding->IsWalkable(next))
	{
		for (uint i = path.Count() - 1; i > 0; --i)
		{
			next = { path.At(i)->x, App->map->WorldToMap(fpPosition.x, (int)round(fpPosition.y)).y + 3 };
			if (!App->pathfinding->IsWalkable(next))
			{
				ret = next;
				break;
			}
		}
	}
	return ret;
}

//Calculates the jump if it is on the distance limit
int j1LandEnemy::CalculateJump(iPoint destination)
{
	iPoint nextToWorld = { destination.x, destination.y + 2 };

	return(abs((nextToWorld.DistanceTo({ (int)round(fpPosition.x), (int)round(fpPosition.y) }))));
}

//Makes the enemy jump if he is not jumping & there's a tile avaliable to jump
bool j1LandEnemy::JumpLogic()
{
	if (path.Count() <= 1)
		return false;

	if (App->pathfinding->IsWalkable({ path.At(path.Count() - 1)->x, path.At(path.Count() - 1)->y + 2 }) && !falling)
	{
		iPoint nextTile = NextWalkableTile();
		if (nextTile.x != -1 && nextTile.y != -1)
		{
			if (!falling)
			{
				if (CalculateJump(App->map->MapToWorld(nextTile.x, nextTile.y)) < jumpDistance)
				{
					fpSpeed.y = -jumpForce;
				}
				else
					return true;
			}
		}
		else
			return true;
	}

	return false;
}

//Moves the entity
void j1LandEnemy::Move(bool toPlayer, float dt)
{
	//Get the next tile to move
	iPoint next = App->map->MapToWorld(path.At(path.Count() - 1)->x, path.At(path.Count() - 1)->y);

	//If the tile is a tile of distance of the enemy, it moves
	if (abs(fpPosition.x - next.x) > App->map->data.tile_width)
	{
		if (fpPosition.x < next.x)
		{
			if (fpSpeed.x < 0 || (toPlayer &&App->entities->player->fpPosition.x < fpPosition.x))
				fpSpeed.x = 0;

			fpSpeed.x += moveSpeed.x * (dt * VEL_TO_WORLD);
		}
		else if (fpPosition.x > next.x)
		{
			if (fpSpeed.x > 0 || (toPlayer && App->entities->player->fpPosition.x > fpPosition.x))
				fpSpeed.x = 0;
			
			fpSpeed.x -= moveSpeed.x * (dt * VEL_TO_WORLD);
		}
	}
	else
		path.Pop(next);
}

//Update the enemy position
void j1LandEnemy::UpdatePos(float dt)
{
	if (falling)
		fpSpeed.y += fallingSpeed * (dt * VEL_TO_WORLD);

	//If the logic does not demostrate tshe opposite, the player is always falling and not touching the wall
	falling = true;

	//Limit Speed
	LimitSpeed();

	fpPosition.x += fpSpeed.x * dt;
	fpPosition.y += fpSpeed.y * dt;

	//We set the collider in hte player's position
	CalculateCollider(fpPosition);	
}

//When the enemy is idle, it has a defined movement
void j1LandEnemy::TraceFollower()
{

	switch (traceDir)
	{
	case (DIRECTION_RIGHT):

		fpSpeed.x = idleSpeed.x;

		if (fpPosition.x >= trace.x + trace.w - collider->rect.w)
		{
			traceDir = DIRECTION_LEFT;
		}
		break;

	case (DIRECTION_LEFT):

		fpSpeed.x = -idleSpeed.x;

		if (fpPosition.x <= trace.x)
		{
			traceDir = DIRECTION_RIGHT;
		}

		break;

	}
}

//If a collision is detected by the j1Collision, distributes collisions according to it's type
void j1LandEnemy::OnCollision(Collider* entityCol, Collider* coll)
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
void j1LandEnemy::RecalculatePos(SDL_Rect entityColl, SDL_Rect collRect)
{
	//If a collision from various aixs is detected, it determines what is the closets one to exit from
	int directionCheck = CheckCollisionDir(entityColl, collRect);

	//Then we update the player's position & logic according to it's movement & the minimum result that we just calculated
	switch (directionCheck) {
	case DIRECTION_UP:
		fpPosition.y = collRect.y + collRect.h + 2;
		fpSpeed.y = 0;
		break;
	case DIRECTION_DOWN:
		fpPosition.y = collRect.y - entityColl.h;
		fpSpeed.y = 0;
		if (!falling)
		{
			path.Clear();
		}
		falling = false;
		break;
	case DIRECTION_LEFT:
		fpPosition.x = collRect.x + collRect.w;
		if (fpSpeed.x < 0)
			fpSpeed.x = 0;
		break;
	case DIRECTION_RIGHT:
		fpPosition.x = collRect.x - entityColl.w;
		if (fpSpeed.x > 0)
			fpSpeed.x = 0;
		break;
	case DIRECTION_NONE:
		break;
	}
	//We Recalculate the entity's collider with the new position
	
	CalculateCollider(fpPosition);
	
}

//Called when loading a save
bool j1LandEnemy::Load(pugi::xml_node& load)
{
	fpPosition.x = load.child("position").attribute("x").as_float();
	fpPosition.y = load.child("position").attribute("y").as_float();
	fpSpeed.x = load.child("speed").attribute("x").as_float();
	fpSpeed.y = load.child("speed").attribute("y").as_float();

	falling = load.child("falling").attribute("value").as_bool();

	trace.x = load.child("trace").attribute("x").as_uint();
	trace.y = load.child("trace").attribute("y").as_uint();
	trace.w = load.child("trace").attribute("w").as_uint();
	trace.h = load.child("trace").attribute("h").as_uint();


	if (load.child("flip").attribute("value") == 0)
		Flip = SDL_FLIP_NONE;
	else
		Flip = SDL_FLIP_HORIZONTAL;

	return true;
}

//Called when loading a save
bool j1LandEnemy::Save(pugi::xml_node& save) const
{
	pugi::xml_node eneNode;

	eneNode = save.append_child("enemyL");

	//Save all the player's status variables
	eneNode.append_child("position").append_attribute("x") = fpPosition.x;
	eneNode.child("position").append_attribute("y") = fpPosition.y;
	eneNode.append_child("speed").append_attribute("x") = fpSpeed.x;
	eneNode.child("speed").append_attribute("y") = fpSpeed.y;

	eneNode.append_child("falling").append_attribute("value") = falling;

	eneNode.append_child("trace").append_attribute("x") = trace.x;
	eneNode.child("trace").append_attribute("y") = trace.y;
	eneNode.child("trace").append_attribute("w") = trace.w;
	eneNode.child("trace").append_attribute("h") = trace.h;

	eneNode.append_child("flip").append_attribute("value") = Flip;

	return true;
}
