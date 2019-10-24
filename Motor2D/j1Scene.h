#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

class j1Scene : public j1Module
{
public:
	//--------INTERNAL CONTROL---------//
	//Constructor
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

	//--------LOAD & SAVE---------//
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	//--------CAMERA---------//
	//Camer logic
	void Camera();
	//Checks camera limits
	void CheckCameraLimits();

private:
	//--------CAMERA---------//
	//The calculated camera
	fPoint cameraPos;
	//Store the window size
	uint width = 0u;
	uint height = 0u;
	//The limits of the camera
	iPoint  Hlimit = { 0,0 };

};

#endif // __j1SCENE_H__