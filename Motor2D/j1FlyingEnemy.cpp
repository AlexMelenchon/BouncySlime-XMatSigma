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

//oWAFegezRNTwm7pdgahyexTk

j1FlyingEnemy::j1FlyingEnemy() : j1Entity()
{
	this->type = entityType::FLYING_ENEMY;

}

j1FlyingEnemy ::~j1FlyingEnemy()
{}


bool j1FlyingEnemy::Awake(pugi::xml_node& land_node)
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
	

	currentAnimation = &animIdle;

	auxLoader = land_node;
	return true;
}

bool j1FlyingEnemy::Start()
{
	//The enemy's texture load
	Text = App->tex->Load(auxLoader.child("path").text().as_string());

	//Collision load
	App->collision->AddCollider(collider);

	return true;
}

bool j1FlyingEnemy::PreUpdate()
{
	UpdateState();

	return true;
}

bool j1FlyingEnemy::Update(float dt)
{

	timer += dt;

	bool ret = true;
	switch (enemy_state)
	{
	case flying_state::ST_UNKNOWN:
	{
		ret = false;
		break;
	}
	case flying_state::ST_IDLE:
	{
		if (collider->CheckCollision(trace))
		{
			path.Clear();
			TraceFollower(dt);
		}
		else
		{
			if (timer > 1.00f)
			{
				ReturnToStart();
				timer = 0;
			}

			if (path.Count() > 0)
			{
				//The update the enemy's position & speed according to it's logic
				Move();
			}

		}
		break;

		break;
	}
	case flying_state::ST_CHASING:
	{
		if (timer > 0.5f)
		{
			GetPathfinding();
			timer = 0;
		}

		if (path.Count() > 0)
		{
			//The update the enemy's position & speed according to it's logic
			Move();
		}
		break;
	}

	}
	UpdatePos(dt);

	return ret;
}

bool j1FlyingEnemy::ReturnToStart()
{
	path.Clear();

	if (!App->pathfinding->CreatePath(App->map->WorldToMap(int(round(fpPosition.x + 1)), int(round(fpPosition.y + 16))), App->map->WorldToMap(trace.x + 1, trace.y)))
		return false;


	uint pathCount = App->pathfinding->GetLastPath()->Count();

	for (uint i = 0; i < pathCount; i++)
	{
		path.PushBack(*App->pathfinding->GetLastPath()->At(i));
	}

	return true;

}


void j1FlyingEnemy::Move()
{
	iPoint current = App->map->MapToWorld(path.At(path.Count() - 1)->x, path.At(path.Count() - 1)->y);

	if (abs(abs(fpPosition.x) - abs(current.x)) > 16 || abs(abs(fpPosition.y) - abs(current.y)) > 16)
	{
		if (fpPosition.x < current.x)
		{
			fpSpeed.x += 10;
			Flip = SDL_FLIP_HORIZONTAL;
			if (fpSpeed.x < 0)
				fpSpeed.x = 0;
		}
		else if (fpPosition.x > current.x)
		{
			fpSpeed.x -= 10;
			Flip = SDL_FLIP_NONE;
			if (fpSpeed.x > 0)
				fpSpeed.x = 0;
		}

		if (fpPosition.y < current.y)
		{
			fpSpeed.y += 10;
			if (fpSpeed.y < 0)
				fpSpeed.y = 0;
		}

		else if (fpPosition.y > current.y)
		{
			fpSpeed.y -= 10;
			if (fpSpeed.y > 0)
				fpSpeed.y = 0;
		}

	}
	else
		path.Pop(current);



}

void j1FlyingEnemy::TraceFollower(float dt)
{
	if (!landcheck && !flycheck)
	{
		fpSpeed.y = 0;
		fpSpeed.x = 60.0f;

		Flip = SDL_FLIP_HORIZONTAL;

		if (fpPosition.x >= trace.x + trace.w - collider->rect.w)
		{
			landcheck = !landcheck;			
		}
	}

	if (!flycheck && landcheck)
	{
		fpSpeed.x = 0;
		fpSpeed.y = 60;
		if (fpPosition.y > trace.y + trace.h - collider->rect.h)
		{
			flycheck = !flycheck;
		}
	}

	if (flycheck && landcheck)
	{
		fpSpeed.y = 0;
		fpSpeed.x = -60.0f;

		Flip = SDL_FLIP_NONE;

		if (fpPosition.x <= trace.x)
		{
			landcheck = !landcheck;
		}
	}

	if (flycheck && !landcheck)
	{
		fpSpeed.x = 0;
		fpSpeed.y = -60;
		if (fpPosition.y < trace.y)
		{
			flycheck = !flycheck;
		}
	}
}

void j1FlyingEnemy::Draw()
{
	App->render->Blit(Text, (int)round(fpPosition.x), (int)round(fpPosition.y), &currentAnimation->GetCurrentFrame(App->GetDeltaTime()), 1.0f, Flip, 0.0f, (currentAnimation->pivotpos->x), (currentAnimation->GetCurrentFrame(App->GetDeltaTime()).h / 2), scalesize);
}

bool j1FlyingEnemy::PostUpdate(bool debug)
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

bool j1FlyingEnemy::CleanUp()
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

void j1FlyingEnemy::UpdatePos(float dt)
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



void j1FlyingEnemy::OnCollision(Collider* entityCol, Collider* coll)
{
	switch (coll->type) {

	case(COLLIDER_WALL):
		RecalculatePos(entityCol->rect, coll->rect);
		break;
	}
}

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

void j1FlyingEnemy::UpdateState()
{
	if (abs(abs(App->entities->player->fpPosition.x) - abs(fpPosition.x)) < CHASING_DISTANCE && App->entities->player->getState() != ST_DEAD && abs(abs(App->entities->player->fpPosition.y) - abs(fpPosition.y)) < CHASING_DISTANCE)
	{
		enemy_state = flying_state::ST_CHASING;
	}
	else
		enemy_state = flying_state::ST_IDLE;
}


bool j1FlyingEnemy::GetPathfinding()
{
	path.Clear();

	if (!App->pathfinding->CreatePath(App->map->WorldToMap(fpPosition.x, fpPosition.y), App->map->WorldToMap(App->entities->player->fpPosition.x, App->entities->player->fpPosition.y)))
		return false;

	uint pathCount = App->pathfinding->GetLastPath()->Count();
	if (pathCount <= 0) return false;

	for (uint i = 0; i < pathCount; i++)
	{
		path.PushBack(*App->pathfinding->GetLastPath()->At(i));
	}

	return true;
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