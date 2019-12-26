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
	bool Awake(pugi::xml_node&);

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
	//Fade time to change maps
	float mapFadeTime = 0.0f;

	//--------PATHFINDING---------//
	//Sets the debug pathfinding w/ the mouse
	void setDebugPathfinding();
	
	//Blits the debug pathfinding, if exists
	void blitDebugPath() const;

	//--------UI---------//
	//Manages the UI inputs of this module
	void OnGui(UIEventType type, UIFunction func, j1UIelement* userPointer = nullptr);


private:
	//--------CAMERA---------//
	//The calculated camera
	fPoint cameraPos;
	//Store the window size
	uint width = 0u;
	uint height = 0u;
	//The limits of the camera
	iPoint  Hlimit = { 0,0 };


	//--------PATHFNIDING DEBUG---------//
	//The texture to blit the pathfinding on screen
	SDL_Texture* debug_tex;

	//Array to store the debug path made w/ the mouse
	p2DynArray<iPoint> debugPath;
};


#endif // __j1SCENE_H__