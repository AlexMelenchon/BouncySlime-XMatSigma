#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Player.h"

j1Collision::j1Collision()
{
	name.create("collision");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		collidersDebug[i] = nullptr;

	matrix[COLLIDER_NONE][COLLIDER_WALL] = false;
	matrix[COLLIDER_NONE][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_NONE][COLLIDER_START] = false;
	matrix[COLLIDER_NONE][COLLIDER_DEATH] = false;
	matrix[COLLIDER_NONE][COLLIDER_WIN] = false;
	matrix[COLLIDER_NONE][COLLIDER_GOD] = false;
	matrix[COLLIDER_NONE][COLLIDER_NONE] = false;

	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALL][COLLIDER_START] = false;
	matrix[COLLIDER_WALL][COLLIDER_DEATH] = false;
	matrix[COLLIDER_WALL][COLLIDER_WIN] = false;
	matrix[COLLIDER_WALL][COLLIDER_GOD] = false;
	matrix[COLLIDER_WALL][COLLIDER_NONE] = false;


	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_START] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_DEATH] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_WIN] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_GOD] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_NONE] = false;

	matrix[COLLIDER_DEATH][COLLIDER_WALL] = false;
	matrix[COLLIDER_DEATH][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_DEATH][COLLIDER_START] = false;
	matrix[COLLIDER_DEATH][COLLIDER_DEATH] = false;
	matrix[COLLIDER_DEATH][COLLIDER_WIN] = false;
	matrix[COLLIDER_DEATH][COLLIDER_GOD] = false;
	matrix[COLLIDER_DEATH][COLLIDER_NONE] = false;

	matrix[COLLIDER_WIN][COLLIDER_WALL] = false;
	matrix[COLLIDER_WIN][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WIN][COLLIDER_START] = false;
	matrix[COLLIDER_WIN][COLLIDER_DEATH] = false;
	matrix[COLLIDER_WIN][COLLIDER_WIN] = false;
	matrix[COLLIDER_WIN][COLLIDER_GOD] = true;
	matrix[COLLIDER_WIN][COLLIDER_NONE] = false;

	matrix[COLLIDER_GOD][COLLIDER_WALL] = false;
	matrix[COLLIDER_GOD][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_GOD][COLLIDER_START] = false;
	matrix[COLLIDER_GOD][COLLIDER_DEATH] = false;
	matrix[COLLIDER_GOD][COLLIDER_WIN] = true;
	matrix[COLLIDER_GOD][COLLIDER_GOD] = false;
	matrix[COLLIDER_GOD][COLLIDER_NONE] = false;
}

// Destructor
j1Collision::~j1Collision()
{}

bool j1Collision::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (collidersDebug[i] != nullptr && collidersDebug[i]->to_delete == true)
		{
			delete collidersDebug[i];
			collidersDebug[i] = nullptr;
		}
	}

	return true;
}

// Called before render is available
bool j1Collision::Update(float dt)
{
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (collidersDebug[i] == nullptr)
			continue;

		c1 = collidersDebug[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (collidersDebug[k] == nullptr)
				continue;

			c2 = collidersDebug[k];

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

bool j1Collision::PostUpdate()
{
	DebugDraw();

	return true;
}

void j1Collision::DebugDraw()
{
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (collidersDebug[i] == nullptr)
			continue;

		switch (collidersDebug[i]->type)
		{
		case COLLIDER_WALL:  //blue
			App->render->DrawQuad(collidersDebug[i]->rect, 0, 0, 255, alpha);
			break;
		case COLLIDER_PLAYER: // green
			App->render->DrawQuad(collidersDebug[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_DEATH: // red
			App->render->DrawQuad(collidersDebug[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_WIN: // white
			App->render->DrawQuad(collidersDebug[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_GOD: // white
			App->render->DrawQuad(collidersDebug[i]->rect, 255, 255, 0, alpha);
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
		if (collidersDebug[i] != nullptr)
		{
			delete collidersDebug[i];
			collidersDebug[i] = nullptr;
		}
	}

	return true;
}

bool j1Collision::CleanMap()
{
	LOG("Freeing map colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (collidersDebug[i] != nullptr && (collidersDebug[i]->type != COLLIDER_PLAYER && collidersDebug[i]->type != COLLIDER_GOD))
		{
			delete collidersDebug[i];
			collidersDebug[i] = nullptr;
		}
	}

	return true;
}

void j1Collision::AddControlCollider(Collider* collider)
{

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (collidersDebug[i] == nullptr)
		{
			collidersDebug[i] = collider;
			break;
		}
	}

}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return !((this->rect.x + this->rect.w < r.x || r.x + r.w < this->rect.x) || (this->rect.y + this->rect.h < r.y || r.y + r.h < this->rect.y));
}