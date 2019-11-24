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

bool j1LandEnemy::Awake(pugi::xml_node &)
{

	return false;
}

bool j1LandEnemy::Start()
{
	return false;
}

bool j1LandEnemy::PreUpdate()
{
	UpdateState();

	return false;
}

bool j1LandEnemy::Update(float dt)
{

	switch (enemy_state)
	{
	case state::ST_UNKNOWN:

		break;

	case state::ST_IDLE:

		break;

	case state::ST_CHASING:

		App->pathfinding->CreatePath(App->map->WorldToMap(fpPosition.x, fpPosition.y), App->map->WorldToMap(App->entities->player->fpPosition.x, App->entities->player->fpPosition.y));
		path = *App->pathfinding->GetLastPath();



	}
	return false;
}

bool j1LandEnemy::PostUpdate()
{
	return false;
}

bool j1LandEnemy::CleanUp()
{
	return false;
}



void j1LandEnemy::OnCollision(Collider* entityCol, Collider* coll)
{
	switch (coll->type) {

	case(COLLIDER_WALL):
		RecalculatePos(entityCol->rect, coll->rect);
		break;
	case(COLLIDER_PLAYER):
		App->entities->player->inputs.add(IN_DEATH);
		break;
	}
}

void j1LandEnemy::RecalculatePos(SDL_Rect entityColl, SDL_Rect collRect)
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
	
	
	
}

void j1LandEnemy::UpdateState()
{
	if (abs(abs(App->entities->player->fpPosition.x) - abs(fpPosition.x)) < CHASING_DISTANCE)
	{
		enemy_state = state::ST_CHASING;
	}
	else
		enemy_state = state::ST_IDLE;
}
