#include "j1App.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Entity.h"
#include "j1Collision.h"

//Constructor
j1Entity::j1Entity()
{
}
//Secondary Constructor
j1Entity::j1Entity(entityType type) : type(type)
{
}
//Destructor
j1Entity::~j1Entity()
{
}

// Limits the entity's speed in both axis
void j1Entity::LimitSpeed(float dt)
{
	//X+
	if (fpSpeed.x > fpMaxSpeed.x * dt)
	{
		fpSpeed.x = fpMaxSpeed.x * dt;
	}

	//X-
	if (fpSpeed.x < -fpMaxSpeed.x * dt)
	{
		fpSpeed.x = -fpMaxSpeed.x * dt;
	}

	//Y+
	if (fpSpeed.y > fpMaxSpeed.y * dt)
	{
		fpSpeed.y = fpMaxSpeed.y * dt;
	}

	//Y-
	if (fpSpeed.y < -fpMaxSpeed.y * dt)
	{
		fpSpeed.y = -fpMaxSpeed.y * dt;
	}
}

//Resets the entity's movement
void j1Entity::ReSetMovement()
{
	fpSpeed = { 0,0 };
	fAccel = 0;
}

//Changes the player's position
void j1Entity::SetPos(int x, int y)
{
	fpPosition.x = x;
	fpPosition.y = y;
}

//Sets the collider to the player's position
void j1Entity::CalculateCollider(fPoint pos)
{
	collider->SetPos((int)fpPosition.x, (int)fpPosition.y);
}

//Blits the entity into the world
void j1Entity::Draw()
{
	App->render->Blit(Text, (int)round(fpPosition.x), (int)round(fpPosition.y), &currentAnimation->GetCurrentFrame(App->GetDeltaTime()), 1.0f, Flip, 0.0f, (currentAnimation->pivotpos->x), (currentAnimation->GetCurrentFrame(App->GetDeltaTime()).h / 2));
}

//Calculates if the entity is flipped or not
void j1Entity::FlipControl()
{
	if (fpSpeed.x > flipSpeed)
		Flip = SDL_FLIP_NONE;

	else if (fpSpeed.x < -flipSpeed)
		Flip = SDL_FLIP_HORIZONTAL;
}

//Checks the direction of the collision
int j1Entity::CheckCollisionDir(SDL_Rect playerRect, SDL_Rect collRect)
{
	//Determines the direction of the collision
	//Calculates distances from the player to the collision
	int collDiference[DIRECTION_MAX];
	collDiference[DIRECTION_LEFT] = (collRect.x + collRect.w) - playerRect.x;
	collDiference[DIRECTION_RIGHT] = (playerRect.x + playerRect.w) - collRect.x;
	collDiference[DIRECTION_UP] = (collRect.y + collRect.h) - playerRect.y;
	collDiference[DIRECTION_DOWN] = (playerRect.y + playerRect.h) - collRect.y;


	//If a collision from various aixs is detected, it determines what is the closets one to exit from
	int directionCheck = DIRECTION_NONE;

	for (int i = 0; i < DIRECTION_MAX; ++i)
	{
		if (directionCheck == DIRECTION_NONE)
			directionCheck = i;
		else if ((collDiference[i] < collDiference[directionCheck]))
			directionCheck = i;
	}

	return directionCheck;
}