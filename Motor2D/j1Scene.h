#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1Console.h"

struct SDL_Texture;
class j1Timer;

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

	//Resets the Camera
	void ResetCamera();

	//--------PATHFINDING---------//
	//Sets the debug pathfinding w/ the mouse
	void setDebugPathfinding();

	//Blits the debug pathfinding, if exists
	void blitDebugPath() const;

	//--------UI---------//
	//Manages the UI inputs of this module
	void OnGui(UIEventType type, UIFunction func, j1UIelement* userPointer = nullptr,  const char* bufferText = "");

	void UIInGameUpdate();

	void UITimeUpdate();

	void MenusLoad(UIFunction func);



	//--------GAMEPLAY RELATED---------//
	//Checks if the maximum score has been surpassed
	bool CheckMaxScore();

	//Called when player loses a life
	void LoseALife();

	//Called when player gets a coin
	void CoinUp();

	//The amount of lifes the player has remaining
	uint lifes = 0u;

	//The amount of coins the player has recollected
	uint coins = 0u;

	// The score in the run of the player
	uint score = 0u;

	// The maximum point achieved in a run
	uint maxScore = 0u;

	//--------UI LOGIC---------//
	//Used to keep the console in check
	j1Console* console = nullptr;

	//The list of commands of the console
	p2List<p2SString*> CommandList;


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

	//Elements
	j1UIelement* pause = nullptr;
	j1UIelement* ui_lifes = nullptr;
	j1UIelement* ui_coins = nullptr;
	j1UIelement* ui_score = nullptr;
	j1UIelement* ui_maxScore = nullptr;
	j1UIelement* ui_time = nullptr;

	//Text to blit into screen
	char lifes_text[2];
	char coins_text[2];
	char score_text[5];
	char maxScore_text[5];
	char time_text[10];

	//--------INTERNAL CONTROL---------//
	//Node for saving the MaxScore
	pugi::xml_node		sceneConfig;


	//--------UI LOGIC---------//
	//Used to load/unload the options submenu
	UIFunction lastcall;

	//"Ghost" parent to control the other elements on screen
	j1UIelement* parent = nullptr;

	//Audio
	FX click;

	//--------GAMEPLAY RELATED---------//

	//The current run time of the run
	j1Timer time;

	//The default lifes given at the start
	uint startingLifes = 0u;

};


#endif // __j1SCENE_H__
