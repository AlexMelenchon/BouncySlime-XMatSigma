#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	 bool Start();

	 // Called to reset the scene
	 bool Reset(const char* map);

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//Loads and saves the scene
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	//Camera logic
	void Camera();

private:
	fPoint cameraPos;
	uint width = 0u;
	uint height = 0u;
	int	 scale = 0;
	iPoint  Hlimit = { 0,0 };
	void CheckCameraLimits();

};

#endif // __j1SCENE_H__