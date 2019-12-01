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
void j1Entity::LimitSpeed()
{
	//X+
	if (fpSpeed.x > fpMaxSpeed.x)
	{
		fpSpeed.x = fpMaxSpeed.x;
	}

	//X-
	else if (fpSpeed.x < -fpMaxSpeed.x)
	{
		fpSpeed.x = -fpMaxSpeed.x;
	}

	//Y+
	if (fpSpeed.y > fpMaxSpeed.y)
	{
		fpSpeed.y = fpMaxSpeed.y;
	}

	//Y-
	else if (fpSpeed.y < -fpMaxSpeed.y)
	{
		fpSpeed.y = -fpMaxSpeed.y;
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

void j1Entity::SetTrace(SDL_Rect trace_range)
{
	trace.x = trace_range.x;
	trace.y = trace_range.y;
	trace.w = trace_range.w;
	trace.h = trace_range.h;
}

//Sets the collider to the player's position
void j1Entity::CalculateCollider(fPoint pos)
{
	collider->SetPos((int)fpPosition.x, (int)fpPosition.y);
}

//Blits the entity into the world
void j1Entity::Draw()
{
	App->render->Blit(Text, (int)round(fpPosition.x), (int)round(fpPosition.y), &currentAnimation->GetCurrentFrame(App->GetDeltaTime()), 1.0f, Flip, 0.0f, (currentAnimation->pivotpos->x), (currentAnimation->GetCurrentFrame(App->GetDeltaTime()).h / 2),scalesize);
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
int j1Entity::CheckCollisionDir(SDL_Rect entityRect, SDL_Rect collRect)
{
	//Determines the direction of the collision
	//Calculates distances from the player to the collision
	int collDiference[DIRECTION_MAX];
	collDiference[DIRECTION_LEFT] = (collRect.x + collRect.w) - entityRect.x;
	collDiference[DIRECTION_RIGHT] = (entityRect.x + entityRect.w) - collRect.x;
	collDiference[DIRECTION_UP] = (collRect.y + collRect.h) - entityRect.y;
	collDiference[DIRECTION_DOWN] = (entityRect.y + entityRect.h) - collRect.y;


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

//Smoothly slows an speed axis
float j1Entity::DeAccel(float speedAxis, float grade)
{
		speedAxis = ((speedAxis / grade) * (App->GetDeltaTime() * VEL_TO_WORLD));

	return speedAxis;
}