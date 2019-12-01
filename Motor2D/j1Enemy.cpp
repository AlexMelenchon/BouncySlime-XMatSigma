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


bool j1Enemy::PreUpdate()
{
	UpdateState();

	return true;
}

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



void j1Enemy::UpdateState()
{
	if (abs(abs(App->entities->player->fpPosition.x) - abs(fpPosition.x)) < chasingDistance && App->entities->player->getState() != ST_DEAD && abs(abs(App->entities->player->fpPosition.y) - abs(fpPosition.y)) < chasingDistance)
	{
		state = enemy_state::ST_CHASING;
	}
	else
		state = enemy_state::ST_IDLE;
}


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


bool j1Enemy::GetPathfinding(fPoint destination, bool tilelimit)
{
	path.Clear();

	if (!App->pathfinding->CreatePath(App->map->WorldToMap(int(round(fpPosition.x + 1)), int(round(fpPosition.y + App->map->data.tile_height))), App->map->WorldToMap(int(round(destination.x)), int(round(destination.y)))))
		return false;

	uint pathCount = App->pathfinding->GetLastPath()->Count();

	if (pathCount <= 0 || (tilelimit && pathCount > chasingTiles)) return false;

	for (uint i = 0; i < pathCount; i++)
	{
		path.PushBack(*App->pathfinding->GetLastPath()->At(i));
	}

	return true;
}