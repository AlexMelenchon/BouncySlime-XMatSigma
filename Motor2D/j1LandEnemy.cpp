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


j1LandEnemy::j1LandEnemy() : j1Entity()
{	
	this->type = entityType::LAND_ENEMY;
	
}

j1LandEnemy ::~j1LandEnemy()
{}


bool j1LandEnemy::Awake(pugi::xml_node &awake)
{
	SDL_Rect entityRect = { 0,0,0,0 };
	entityRect.w = 37;
	entityRect.h = 38;
	collider = new Collider(entityRect, COLLIDER_ENEMY, this);
	
	return true;
}

bool j1LandEnemy::Start()
{
	Text = App->tex->Load("textures/player/landEnemy.png");

	//Collision load
	App->collision->AddCollider(collider);

	return true;
}

bool j1LandEnemy::PreUpdate()
{
	UpdateState();

	return true;
}

bool j1LandEnemy::Update(float dt)
{
	currentAnimation = App->entities->player->currentAnimation;

	bool ret = true;
	switch (enemy_state)
	{
	case state::ST_UNKNOWN:
		{
		ret = false;
		break;
		}
	case state::ST_IDLE:
		{

		break;
		}
	case state::ST_CHASING:
		{
		GetPathfinding();
		
		iPoint current = App->map->MapToWorld(path.At(path.Count() - 1)->x, path.At(path.Count() - 1)->y);


		//The update the player's position & speed according to it's logic
		if (fpPosition.x < current.x)
			fpSpeed.x = 30;
		if (fpPosition.x > current.x)
			fpSpeed.x = -30;

		path.Pop(current);
		break;
		}
		
	}
	UpdatePos(dt);
	
	return ret;
}

bool j1LandEnemy::PostUpdate(bool debug)
{
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

bool j1LandEnemy::CleanUp()
{
	App->tex->UnLoad(Text);
	path.Clear();
	return true;
}

void j1LandEnemy::UpdatePos(float dt)
{
	//If the logic does not demostrate tshe opposite, the player is always falling and not touching the wall
	falling = true;

	//path.Flip();
	
	
	
	//fpSpeed.y += fAccel * dt;
	

	//Limit Speed
	//LimitSpeed(dt);

	fpPosition.x += fpSpeed.x * dt;
	fpPosition.y += fpSpeed.y * dt;

	
	
	//We set the collider in hte player's position
	CalculateCollider(fpPosition);
	
}



void j1LandEnemy::OnCollision(Collider* entityCol, Collider* coll)
{
	switch (coll->type) {

	case(COLLIDER_WALL):
		RecalculatePos(entityCol->rect, coll->rect);
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
	
	CalculateCollider(fpPosition);
	
}

void j1LandEnemy::UpdateState()
{
	if (abs(abs(App->entities->player->fpPosition.x) - abs(fpPosition.x)) < CHASING_DISTANCE && App->entities->player->getState() != ST_DEAD)
	{
		enemy_state = state::ST_CHASING;
	}
	else
		enemy_state = state::ST_IDLE;
}


bool j1LandEnemy::GetPathfinding()
{
	path.Clear();
	App->pathfinding->CreatePath(App->map->WorldToMap(fpPosition.x, fpPosition.y), App->map->WorldToMap(App->entities->player->fpPosition.x, App->entities->player->fpPosition.y));

	uint pathCount = App->pathfinding->GetLastPath()->Count();
	if (pathCount <= 0) return false;
	for (uint i = 0; i < pathCount; i++)
	{
		path.PushBack(*App->pathfinding->GetLastPath()->At(i));
	}


	return true;

}
