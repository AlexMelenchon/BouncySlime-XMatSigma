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

//Constructor
j1ParticleShuriken::j1ParticleShuriken() : j1Entity()
{
	this->type = entityType::SHURIKEN;

	if (App->entities->shuriken == nullptr)
		App->entities->shuriken = this;
}

//Destructor
j1ParticleShuriken ::~j1ParticleShuriken()
{
	path.~p2DynArray();
}


// Called before render is available
bool j1ParticleShuriken::Awake(pugi::xml_node& shuriken_node)
{
	//movement
	pugi::xml_node movement = shuriken_node.child("movement");
	returnTimer = movement.child("returnTimer").text().as_float();
	speedReduction = movement.child("speedReduction").text().as_float();
	minimumSpeed = movement.child("minimumSpeed").text().as_float();
	friction = movement.child("friction").text().as_float();
	returnSpeed.x = movement.child("returnSpeed").attribute("x").as_float();
	returnSpeed.y = movement.child("returnSpeed").attribute("y").as_float();
	defaultSpeed = movement.child("defaultSpeed").text().as_float();
	deAccelGrade = movement.child("deAccelGrade").text().as_float();

	//Create the player's collider
	scalesize = shuriken_node.child("collision").child("scalesize").text().as_uint();

	SDL_Rect particleRect = { 0,0,0,0 };
	particleRect.w = shuriken_node.child("collision").child("collider").attribute("w").as_float() * scalesize;
	particleRect.h = shuriken_node.child("collision").child("collider").attribute("h").as_float() * scalesize;

	//collider load
	collider = new Collider(particleRect, COLLIDER_SHURIKEN, this);

	//animation load
	pugi::xml_node animIterator = shuriken_node.child("animations").child("animation");
	anim.loadAnimation(animIterator, "shuriken");

	currentAnimation = &anim;

	return true;
}

// Called before the first frame
bool j1ParticleShuriken::Start()
{
	//The enemy's texture load
	Text = App->entities->player_tex;

	//Collision load
	App->collision->AddCollider(collider);

	GetInitalSpeed();

	return true;
}

// Called each loop iteration
bool j1ParticleShuriken::PreUpdate()
{
	BROFILER_CATEGORY("Particle Shuriken Ppre-Update", Profiler::Color::OldLace)
		UpdateState();
	return true;
}

// Called each loop iteration
bool j1ParticleShuriken::Update(float dt)
{
	BROFILER_CATEGORY("Particle Shuriken Update", Profiler::Color::OldLace)
		bool ret = true;

	//Update the timer
	timer += dt;


	switch (state)
	{
	case shuri_state::ST_LAUNCH:

	{
		if (abs(fpSpeed.x) < minimumSpeed && abs(fpSpeed.y) < minimumSpeed)
		{
			if (path.Count() == NULL)
			{
				ReturnToPlayerPath();
				canPickUp = true;
				timer = 0;
			}
		}

		fpSpeed.x -= DeAccel(fpSpeed.x, friction);
		fpSpeed.y -= DeAccel(fpSpeed.y, friction);

		break;
	}

	case shuri_state::ST_RETURNING:
	{
		if (path.Count() > 0)
			MoveToPlayer(dt);

		if (timer > returnTimer)
		{
			ReturnToPlayerPath();
			timer = 0;
		}
		break;
	}
	}
	//Play the fx meanwhile in the air
	 //App->audio->PlayFx(in_air.id,0,0);

	 //Update shuriken position
	UpdatePos(dt);

	return ret;
}

//Updates the shuriken state
void j1ParticleShuriken::UpdateState()
{
	if (!canPickUp)
		state = shuri_state::ST_LAUNCH;
	else
		state = shuri_state::ST_RETURNING;
}

//Moves the shuriken to the player
void j1ParticleShuriken::MoveToPlayer(float dt)
{
	iPoint current = App->map->MapToWorld(path.At(path.Count() - 1)->x, path.At(path.Count() - 1)->y - 0.75);

	if (abs(abs(fpPosition.x) - abs(current.x)) > App->map->data.tile_height || abs(abs(fpPosition.y) - abs(current.y)) > App->map->data.tile_height)
	{
		if (fpPosition.x < current.x)
		{
			if (fpSpeed.x <0)
				fpSpeed.x -= DeAccel(fpSpeed.x, deAccelGrade);

			fpSpeed.x += returnSpeed.x * (dt * VEL_TO_WORLD);
		}
		else if (fpPosition.x > current.x)
		{
			if (fpSpeed.x > 0)
				fpSpeed.x -= DeAccel(fpSpeed.x, deAccelGrade);

			fpSpeed.x -= returnSpeed.x * (dt * VEL_TO_WORLD);
		}

		if (fpPosition.y < current.y)
		{
			if (fpSpeed.y < 0)
				fpSpeed.y -= DeAccel(fpSpeed.y, deAccelGrade);

			fpSpeed.y += returnSpeed.y * (dt * VEL_TO_WORLD);
		}

		else if (fpPosition.y > current.y)
		{
			if (fpSpeed.y > 0)
				fpSpeed.y -= DeAccel(fpSpeed.y, deAccelGrade);

			fpSpeed.y -= returnSpeed.y * (dt * VEL_TO_WORLD);

		}

	}
	else
		path.Pop(current);

}

// Called each loop iteration
bool j1ParticleShuriken::PostUpdate(bool debug)
{
	BROFILER_CATEGORY("Particle Shuriken Post-Update", Profiler::Color::OldLace)
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


// Called before quitting
bool j1ParticleShuriken::CleanUp()
{
	if (collider != nullptr)
	{
		collider->to_delete = true;
		collider = nullptr;
	}

	App->entities->shuriken = nullptr;
	Text = nullptr;

	path.Clear();

	return true;
}

//Updates Shuriken Position
void j1ParticleShuriken::UpdatePos(float dt)
{
	//If the logic does not demostrate tshe opposite, the player is always falling and not touching the wall
	falling = true;

	fpPosition.x += fpSpeed.x * dt;
	fpPosition.y += fpSpeed.y * dt;

	//We set the collider in hte player's position
	CalculateCollider(fpPosition);
}

//Determines Shuriken initial Spped
void j1ParticleShuriken::GetInitalSpeed()
{
	//X-
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		fpSpeed.x = -defaultSpeed;
	}
	//X+
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		fpSpeed.x = defaultSpeed;
	}


	//Y+
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		fpSpeed.y = -defaultSpeed;
	}
	//Y-
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		fpSpeed.y = defaultSpeed;
	}

	if (fpSpeed.Mod() == 0)
		fpSpeed.x = defaultSpeed;
}

//Gets the path to return to the player
bool j1ParticleShuriken::ReturnToPlayerPath()
{
	path.Clear();

	if (!App->pathfinding->CreatePath(App->map->WorldToMap(int(round(fpPosition.x + 1)), int(round(fpPosition.y + App->map->data.tile_height))), App->map->WorldToMap(int(round(App->entities->player->fpPosition.x)), int(round(App->entities->player->fpPosition.y + App->entities->player->collider->rect.w / 2)))))
		return false;

	uint pathCount = App->pathfinding->GetLastPath()->Count();

	if (pathCount <= 0)
	{
		path.Clear();
		return false;
	}

	for (uint i = 0; i < pathCount; i++)
	{
		path.PushBack(*App->pathfinding->GetLastPath()->At(i));
	}

	return true;

}

//If a collision is detected by the j1Collision, distributes collisions according to it's type
void j1ParticleShuriken::OnCollision(Collider* entityCol, Collider* coll)
{
	switch (coll->type) {

	case(COLLIDER_WALL):
		if(!canPickUp)
		RecalculatePos(entityCol->rect, coll->rect);
		break;

	case(COLLIDER_DEATH):
		if (!canPickUp)
		RecalculatePos(entityCol->rect, coll->rect);
		break;

	case(COLLIDER_ENEMY):
		coll->to_delete = true;
		App->audio->PlayFx(App->entities->player->shuriken_hit.id);
		App->scene->score += 75;
		break;

	case(COLLIDER_PLAYER):
		if (canPickUp)
			entityCol->to_delete = true;
		break;
	case(COLLIDER_GOD):
		if (canPickUp)
			entityCol->to_delete = true;
		break;
	}
}

//Calculate the collisions with the enviroment
void j1ParticleShuriken::RecalculatePos(SDL_Rect entityColl, SDL_Rect collRect)
{
	//If a collision from various aixs is detected, it determines what is the closets one to exit from
	int directionCheck = CheckCollisionDir(entityColl, collRect);

	//Then we update the player's position & logic according to it's movement & the minimum result that we just calculated
	switch (directionCheck) {
	case DIRECTION_UP:
		fpPosition.y = collRect.y + collRect.h + 2;
		fpSpeed.y *= -speedReduction;

		break;
	case DIRECTION_DOWN:
		fpPosition.y = collRect.y - entityColl.h;
		fpSpeed.y *= -speedReduction;
		break;
	case DIRECTION_LEFT:
		fpPosition.x = collRect.x + collRect.w;
		fpSpeed.x *= -speedReduction;

		break;
	case DIRECTION_RIGHT:
		fpPosition.x = collRect.x - entityColl.w;
		fpSpeed.x *= -speedReduction;
		break;
	case DIRECTION_NONE:
		break;
	}

	//We Recalculate the entity's collider with the new position
	CalculateCollider(fpPosition);

}
