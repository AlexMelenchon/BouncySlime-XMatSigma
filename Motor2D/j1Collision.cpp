#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Player.h"

//Constructor
j1Collision::j1Collision()
{
	name.create("collision");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	//Matrix creation: decide which type of colliders will collide

	matrix[COLLIDER_NONE][COLLIDER_WALL] = false;
	matrix[COLLIDER_NONE][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_NONE][COLLIDER_START] = false;
	matrix[COLLIDER_NONE][COLLIDER_DEATH] = false;
	matrix[COLLIDER_NONE][COLLIDER_WIN] = false;
	matrix[COLLIDER_NONE][COLLIDER_GOD] = false;
	matrix[COLLIDER_NONE][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_NONE][COLLIDER_SHURIKEN] = false;
	matrix[COLLIDER_NONE][COLLIDER_NONE] = false;
	

	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALL][COLLIDER_START] = false;
	matrix[COLLIDER_WALL][COLLIDER_DEATH] = false;
	matrix[COLLIDER_WALL][COLLIDER_WIN] = false;
	matrix[COLLIDER_WALL][COLLIDER_GOD] = false;
	matrix[COLLIDER_WALL][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_WALL][COLLIDER_SHURIKEN] = true;
	matrix[COLLIDER_WALL][COLLIDER_NONE] = false;


	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_START] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_DEATH] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_WIN] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_GOD] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_SHURIKEN] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_NONE] = false;

	matrix[COLLIDER_DEATH][COLLIDER_WALL] = false;
	matrix[COLLIDER_DEATH][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_DEATH][COLLIDER_START] = false;
	matrix[COLLIDER_DEATH][COLLIDER_DEATH] = false;
	matrix[COLLIDER_DEATH][COLLIDER_WIN] = false;
	matrix[COLLIDER_DEATH][COLLIDER_GOD] = false;
	matrix[COLLIDER_DEATH][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_DEATH][COLLIDER_SHURIKEN] = true;
	matrix[COLLIDER_DEATH][COLLIDER_NONE] = false;

	matrix[COLLIDER_WIN][COLLIDER_WALL] = false;
	matrix[COLLIDER_WIN][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WIN][COLLIDER_START] = false;
	matrix[COLLIDER_WIN][COLLIDER_DEATH] = false;
	matrix[COLLIDER_WIN][COLLIDER_WIN] = false;
	matrix[COLLIDER_WIN][COLLIDER_GOD] = true;
	matrix[COLLIDER_WIN][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_WIN][COLLIDER_SHURIKEN] = false;
	matrix[COLLIDER_WIN][COLLIDER_NONE] = false;

	matrix[COLLIDER_GOD][COLLIDER_WALL] = false;
	matrix[COLLIDER_GOD][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_GOD][COLLIDER_START] = false;
	matrix[COLLIDER_GOD][COLLIDER_DEATH] = false;
	matrix[COLLIDER_GOD][COLLIDER_WIN] = true;
	matrix[COLLIDER_GOD][COLLIDER_GOD] = false;
	matrix[COLLIDER_GOD][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_GOD][COLLIDER_SHURIKEN] = false;
	matrix[COLLIDER_GOD][COLLIDER_NONE] = false;



	matrix[COLLIDER_ENEMY][COLLIDER_WALL] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_START] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_DEATH] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_WIN] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_GOD] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_SHURIKEN] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_NONE] = false;

	matrix[COLLIDER_SHURIKEN][COLLIDER_WALL] = true;
	matrix[COLLIDER_SHURIKEN][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_SHURIKEN][COLLIDER_START] = false;
	matrix[COLLIDER_SHURIKEN][COLLIDER_DEATH] = true;
	matrix[COLLIDER_SHURIKEN][COLLIDER_WIN] = false;
	matrix[COLLIDER_SHURIKEN][COLLIDER_GOD] = false;
	matrix[COLLIDER_SHURIKEN][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_SHURIKEN][COLLIDER_SHURIKEN] = false;
	matrix[COLLIDER_SHURIKEN][COLLIDER_NONE] = false;
	
}

// Destructor
j1Collision::~j1Collision()
{}

// Called each loop iteration
bool j1Collision::PreUpdate()
{
	BROFILER_CATEGORY("Collision Pre-Update", Profiler::Color::IndianRed)

	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

// Called each loop iteration
bool j1Collision::Update(float dt)
{
	BROFILER_CATEGORY("Collision Update", Profiler::Color::IndianRed)

	Collider* c1;
	Collider* c2;

	//Checks if all colliders are colliding with each other
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

				if (c1->CheckCollision(c2->rect) == true)
				{
					if (matrix[c1->type][c2->type] && c1->callback)
						c1->callback->OnCollision(c1, c2);

					if (matrix[c2->type][c1->type] && c2->callback)
						c2->callback->OnCollision(c2, c1);
				}
			
		}
	}

	return true;
}

// Called each loop iteration
bool j1Collision::PostUpdate()
{
	BROFILER_CATEGORY("Collision Post-Update", Profiler::Color::IndianRed)

	DebugDraw();

	return true;
}

void j1Collision::DebugDraw()
{

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_WALL:  //blue
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case COLLIDER_PLAYER: // green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_DEATH: // red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_WIN: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_GOD: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 0, alpha);
			break;
		case COLLIDER_ENEMY: // white
			App->render->DrawQuad(colliders[i]->rect, 125, 125, 0, alpha);
			break;
		case COLLIDER_SHURIKEN:
			App->render->DrawQuad(colliders[i]->rect, 225, 125, 70, alpha);
			break;
		}
	}
}

// Called before quitting
bool j1Collision::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

//Cleans the map colliders only
bool j1Collision::CleanMap()
{
	LOG("Freeing map colliders");

		for (uint i = 0; i < MAX_COLLIDERS; ++i)
		{
			if (colliders[i] != nullptr && (colliders[i]->type != COLLIDER_PLAYER && colliders[i]->type != COLLIDER_GOD))
			{
				delete colliders[i];
				colliders[i] = nullptr;
			}
		}


	return true;
}

//Adds a collider
void j1Collision::AddCollider(Collider* collider)
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			colliders[i] = collider;
			break;
		}
	}

}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return !((this->rect.x + this->rect.w < r.x || r.x + r.w < this->rect.x) || (this->rect.y + this->rect.h +1 < r.y || r.y + r.h +1 < this->rect.y));
}