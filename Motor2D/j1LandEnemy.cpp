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


bool j1LandEnemy::Awake(pugi::xml_node& land_node)
{
	//Movement load
	fGravity = land_node.child("movement").child("gravity").text().as_float();

	//Create the player's collider
	SDL_Rect enemyRect = { 0,0,0,0 };
	enemyRect.w = land_node.child("collision").child("collider").attribute("w").as_float();
	enemyRect.h = land_node.child("collision").child("collider").attribute("h").as_float();

	collider = new Collider(enemyRect, COLLIDER_ENEMY, this);

	//Internal variables load
	pathTimer = land_node.child("internal").child("pathTimer").text().as_float();

	pugi::xml_node animIterator = land_node.child("animations").child("animation");
	animIdle.loadAnimation(animIterator, "idle");
	animRun.loadAnimation(animIterator, "run");
	animWall.loadAnimation(animIterator, "wall");
	animJump.loadAnimation(animIterator, "jump");
	animFall.loadAnimation(animIterator, "fall");
	animDeath.loadAnimation(animIterator, "death");

	currentAnimation = &animIdle;

	auxLoader = land_node;
	return true;
}

bool j1LandEnemy::Start()
{
	//The enemy's texture load
	Text = App->tex->Load(auxLoader.child("path").text().as_string());

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
		if (path.Count() > 0)
		{
			iPoint current = App->map->MapToWorld(path.At(path.Count() - 1)->x, path.At(path.Count() - 1)->y);


			//The update the player's position & speed according to it's logic
			if (fpPosition.x < current.x)
				fpSpeed.x = 30;
			if (fpPosition.x > current.x)
				fpSpeed.x = -30;

			path.Pop(current);
		}
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
	if (collider != nullptr)
	{
		collider->to_delete = true;
		collider = nullptr;
	}

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

//Called when loading a save
bool j1LandEnemy::Load(pugi::xml_node& load)
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
	
	//TODO, save pathfinding timer
	//save.append_child("wallTimer").append_attribute("wallJumpTimer") = wallJumpTimer;

	eneNode.append_child("flip").append_attribute("value") = Flip;

	return true;
}