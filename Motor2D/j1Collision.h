#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#define MAX_COLLIDERS 20

#include "j1Module.h"



struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE type;
	j1Module* callback = nullptr;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, j1Module* callback = nullptr) :
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

	void ReSet(int x, int y, int w, int h)
	{
		rect.x = x;
		rect.y = y;
		rect.w = w;
		rect.h = h;
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

	j1Collision();
	~j1Collision();

	bool PreUpdate() ;
	bool Update(float dt) ;
	bool PostUpdate() ;
	bool CleanUp() ;

	void AddControlCollider(Collider*);
	void DebugDraw();

private:

	Collider* collidersDebug[MAX_COLLIDERS];
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	bool debug = false;
};

#endif // __ModuleCollision_H__