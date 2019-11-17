#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "j1Module.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "PugiXml\src\pugixml.hpp"

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1Scene;
class j1Map;
class j1Player;
class j1Collision;
class j1FadeToBlack;
class j1PathFinding;

class j1App
{
public:
	//--------INTERNAL CONTROL---------//
	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGame();
	void SaveGame() const;
	void GetSaveGames(p2List<p2SString>& list_to_fill) const;

	//Gets the difference between the last update (frames)
	//Used in the physics calculations of the game
	float GetDeltaTime() const;

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	//--------SAVE & LOAD ---------//
	//Says that we want to load and iterates with all modules' load functions
	bool LoadGameNow();
	//Says that we want to save and iterates with all modules' save functions
	bool SavegameNow() const;

public:

	//--------MODULES---------//

	j1Window*			win;
	j1Input*			input;
	j1Render*			render;
	j1Textures*			tex;
	j1Audio*			audio;
	j1Scene*			scene;
	j1Map*				map;
	j1Player*			player;
	j1Collision*		collision;
	j1FadeToBlack*		 fade;
	j1PathFinding*		pathfinding;

	//-----FRAME CONTROL------//
	bool windowTitleControl = false; //Changes the title of the window
	bool pause = false;

	j1Timer* gameTimer = nullptr;
	j1PerfTimer* gamePerfTimer = nullptr;
	j1Timer* lastSecFrames = nullptr;
	j1Timer lastFrameTimer;

	uint64 frame_count = 0u;
	uint last_second_frame_count = 0u;

	uint32 last_frame_ms = 0u;
	uint32 frames_on_last_update = 0u;

	float avg_fps = 0.0f;

	uint capTime = 0u;
	bool frameCap = false;
	float aux = 0.0f;


private:
	//--------INTERNAL CONTROL---------//
	p2List<j1Module*>	modules;
	uint				frames = 0u;
	float				dt = 0.0f;
	int					argc;
	char**				args;

	//--------PROJECT---------//
	p2SString			title;
	p2SString			organization;

	//--------SAVE & LOAD ---------//
	mutable bool		want_to_save;
	bool				want_to_load;
	p2SString			load_game;
	mutable p2SString	save_game;

};

extern j1App* App;

#endif