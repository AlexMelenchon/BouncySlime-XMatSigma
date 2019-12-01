#include "j1App.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Enemy.h"
#include "j1Collision.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"

//Constructor
j1Enemy::j1Enemy()
{
}

//Destructor
j1Enemy::~j1Enemy()
{
}


// Called each loop iteration
bool j1Enemy::PreUpdate()
{
	UpdateState();

	return true;
}

// Called before quitting
bool j1Enemy::CleanUp()
{
	if (collider != nullptr)
	{
		collider->to_delete = true;
		collider = nullptr;
	}

	path.Clear();
	return true;
}


//Changes the state of the enemy
void j1Enemy::UpdateState()
{
	if (abs(abs(App->entities->player->fpPosition.x) - abs(fpPosition.x)) < chasingDistance && App->entities->player->getState() != ST_DEAD && abs(abs(App->entities->player->fpPosition.y) - abs(fpPosition.y)) < chasingDistance)
	{
		state = enemy_state::ST_CHASING;
	}
	else
		state = enemy_state::ST_IDLE;
}

// Called each loop iteration
bool j1Enemy::PostUpdate(bool debug)
{
	
	FlipControl();

	Draw();

	if (debug)
	{
		for (uint i = 0; i < path.Count(); ++i)
		{
			iPoint pos = App->map->MapToWorld(path.At(i)->x, path.At(i)->y);
			App->render->Blit(App->entities->debug_tex, pos.x, pos.y);
		}
	}


	return true;
}

//Return the pathfinding to a map destination, also can be cap or not
bool j1Enemy::GetPathfinding(fPoint destination, bool tilelimit)
{
	//First of all, we clear the path if any
	path.Clear();

	//Then twe calculate the path, if  the origin or the destiantion is not walkable, we exit the func
	if (!App->pathfinding->CreatePath(App->map->WorldToMap(int(round(fpPosition.x + 1)), int(round(fpPosition.y + App->map->data.tile_height))), App->map->WorldToMap(int(round(destination.x)), int(round(destination.y)))))
		return false;

	//We count the path
	uint pathCount = App->pathfinding->GetLastPath()->Count();

	//If it is too long we discard it (although we already calculate it, it is to prevent the enemies from goind crazy or beign to op)
	//We decided to calculate the path either way, since we've a cap on the state machine
	if (pathCount <= 0 || (tilelimit && pathCount > chasingTiles)) return false;

	//If everything is okay, we load the path into de enemy
	for (uint i = 0; i < pathCount; i++)
	{
		path.PushBack(*App->pathfinding->GetLastPath()->At(i));
	}

	return true;
}