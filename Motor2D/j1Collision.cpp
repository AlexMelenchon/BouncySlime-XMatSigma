#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"

j1Collision::j1Collision()
{
	name.create("collision");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		collidersDebug[i] = nullptr;

	matrix[COLLIDER_NONE][COLLIDER_WALL] = false;
	matrix[COLLIDER_NONE][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_NONE][COLLIDER_START] = false;
	matrix[COLLIDER_NONE][COLLIDER_NONE] = false;

	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALL][COLLIDER_START] = false;
	matrix[COLLIDER_WALL][COLLIDER_NONE] = false;


	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_START] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_NONE] = false;

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

	DebugDraw();

	return true;
}

bool j1Collision::PostUpdate()
{
	// Calculate collisions
	Collider* c1;
	Collider* c2;

	//for (uint i = 0; i < MAX_COLLIDERS; ++i)
	//{
	//	// skip empty colliders
	//	if (colliders[i] == nullptr)
	//		continue;

	//	c1 = colliders[i];

	//	// avoid checking collisions already checked
	//	for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
	//	{
	//		// skip empty colliders
	//		if (colliders[k] == nullptr)
	//			continue;

	//		c2 = colliders[k];

	//		if (c1->CheckCollision(c2->rect) == true)
	//		{
	//			if (matrix[c1->type][c2->type] && c1->callback)
	//				c1->callback->OnCollision(c1, c2);

	//			if (matrix[c2->type][c1->type] && c2->callback)
	//				c2->callback->OnCollision(c2, c1);
	//		}
	//	}
	//}

	return true;
}

void j1Collision::DebugDraw()
{
	//if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		//debug = !debug;

	debug = true;

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
	// TODO 0: Return true if there is an overlap
	return !((this->rect.x + this->rect.w < r.x || r.x + r.w < this->rect.x) || (this->rect.y + this->rect.h < r.y || r.y + r.h < this->rect.y));
}