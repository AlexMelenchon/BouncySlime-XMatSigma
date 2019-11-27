#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__


#include "j1Module.h"
#include "j1Entity.h"

#define MAX_COLLIDERS 60

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE type;
	j1Entity* callback = nullptr;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, j1Entity* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{};

	Collider() :
		rect({ 0, 0, 0, 0 }),
		type(COLLIDER_NONE),
		callback(nullptr)
	{};

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	void setType(COLLIDER_TYPE type1)
	{
		type = type1;
	}

	bool CheckCollision(const SDL_Rect& r) const;
};

class j1Collision : public j1Module
{
public:

	//--------INTERNAL CONTROL---------//
	//Constructor
	j1Collision();

	//Destructor
	~j1Collision();

	// Called each loop iteration
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called each loop iteration
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();


	//--------COLLISION ---------//
	//Cleans only the colliders in map
	bool CleanMap();

	//Adds a collider
	void AddCollider(Collider*);

	//Draws the colliders on hte screen
	void DebugDraw();

	//Draw Mode / Activates DebugDraw()
	bool debug = false;

private:

	//Maximum colliders
	int maxColliders = 0;

	//Collider Array
	Collider* colliders[MAX_COLLIDERS];

	//Collider collision matrix
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];

};

#endif // __ModuleCollision_H__