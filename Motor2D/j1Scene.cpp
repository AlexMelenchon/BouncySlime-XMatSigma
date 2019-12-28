#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "j1FadeToBlack.h"
#include "j1Collision.h"
#include "j1EntityManager.h"
#include "j1UIManager.h"
#include "j1MainMenu.h"
#include "j1ConsoleM.h"

#include <stdio.h>

//Constructor
j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& scene_config)
{
	LOG("Loading Scene");
	bool ret = true;

	//Auxiliar used to save the maximum score
	sceneConfig = scene_config;

	//Var load------------
	mapFadeTime = scene_config.child("mapFadeTime").text().as_float();
	startingLifes = scene_config.child("startingLifes").text().as_uint(3);
	maxScore = scene_config.child("maxScore").attribute("value").as_uint(4500);
	click.path = scene_config.child("click").attribute("file").as_string();

	//Command creation--------
	App->console->CreateCommand("god_mode", this, 1, 1, UIFunction::FNC_GODMODE);
	App->console->CreateCommand("map map_name.tmx", this, 2, 2, UIFunction::FNC_LOADMAP);
	App->console->CreateCommand("FPS number (between 30-120)", this, 2, 2, UIFunction::FNC_FPS);


	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	//Create the player
	App->entities->AddEntity(entityType::PLAYER, { 0,0 });

	//Load ui fx
	click.id = App->audio->LoadFx(click.path.GetString());

	//Loads the first map
	Reset(App->map->data.maplist.start->data->name.GetString());

	//Debug texture for the debug pathfinding
	debug_tex = App->entities->debug_tex;

	lifes = startingLifes;
	sprintf_s(lifes_text, 10, "%02d", lifes);

	//UI init-------
	parent = App->ui->AddElement(ui_type::UI_IMAGE, nullptr, { 0,0 }, false, false, true, { 0,0,0,0 }, this, UIFunction::FNC_NONE, drag_axis::MOV_NONE);

	pause = App->ui->AddElement(ui_type::UI_BUTTON, parent, { -10,-10 }, true, false, true, { 220,406,64,64 }, this, UIFunction::FNC_PAUSE);

	App->ui->AddElement(ui_type::UI_IMAGE, nullptr, { 94,10 }, false, false, true, { 1257,532,65,65 });
	ui_lifes = App->ui->AddElement(ui_type::UI_TEXT, nullptr, { 164,30 }, false, false, true, { 0,0,0,0 }, nullptr, UIFunction::FNC_NONE, drag_axis::MOV_NONE, lifes_text);


	App->ui->AddElement(ui_type::UI_IMAGE, nullptr, { 244,10 }, false, false, true, { 1257,605,65,64 });
	ui_coins = App->ui->AddElement(ui_type::UI_TEXT, nullptr, { 314,30 }, false, false, true, { 0,0,0,0 }, nullptr, UIFunction::FNC_NONE, drag_axis::MOV_NONE, "00");

	App->ui->AddElement(ui_type::UI_IMAGE, nullptr, { 394,10 }, false, false, true, { 144,922,65,64 });
	ui_score = App->ui->AddElement(ui_type::UI_TEXT, nullptr, { 464,30 }, false, false, true, { 0,0,0,0 }, nullptr, UIFunction::FNC_NONE, drag_axis::MOV_NONE, "00000");

	App->ui->AddElement(ui_type::UI_IMAGE, nullptr, { 594,10 }, false, false, true, { 737,478,64,64 });
	ui_time = App->ui->AddElement(ui_type::UI_TEXT, nullptr, { 664,30 }, false, false, true, { 0,0,0,0 }, nullptr, UIFunction::FNC_NONE, drag_axis::MOV_NONE, "00000");

	//Console Init------
	App->ui->AddElement(ui_type::UI_CONSOLE, nullptr, { 0,0 }, true, false, false, { 0,0,0,0 }, this, UIFunction::FNC_NONE, drag_axis::MOV_NONE);



	//Gameplay ini--------
	time.Start();

	return true;
}

// Called to reset the scene
bool j1Scene::Reset(const char* map)
{
	//Gets the current window width & height
	App->win->GetWindowSize(width, height);

	//Loads the map
	if (App->map->Load(map) == true)
	{
		int w, h;
		uchar* data = nullptr;
		if (App->map->CreateWalkabilityMap(w, h, &data))
		{
			App->pathfinding->SetMap(w, h, data);
			RELEASE_ARRAY(data);
		}

	}

	// Limit for the end of the map
	Hlimit.x = App->map->data.tile_width * App->map->data.width;
	Hlimit.y = App->map->data.tile_height * App->map->data.height;

	//Start the music
	App->audio->PlayMusic(App->map->data.music.GetString(), 0.0f);

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	BROFILER_CATEGORY("Scene Pre-Update", Profiler::Color::Orange)

		//Camera logic
		Camera();

	// debug pathfing ------------------
	setDebugPathfinding();
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("Scene Update", Profiler::Color::Orange)

		//--------DEBUG---------//

	//Loads the 1st map
		if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
			App->fade->FadeToBlackMap(App->map->data.maplist.start->data->name.GetString(), NULL, mapFadeTime);

	//Loads the 2nd map
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		App->fade->FadeToBlackMap(App->map->data.maplist.At(1)->data->name.GetString(), NULL, mapFadeTime);

	//Loads the 3rd map
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		App->fade->FadeToBlackMap(App->map->data.maplist.At(2)->data->name.GetString(), NULL, mapFadeTime);

	//Reloads current map (a.k.a player's death)
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
		App->fade->FadeToBlackMap(App->map->data.currentmap.GetString(), NULL, mapFadeTime);

	//Saves the game
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	//Loads the game
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	//Activates the debug draw for the ui
	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
		App->ui->debug = !App->ui->debug;

	//Exits to the main menu
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		App->fade->FadeToBlackMod(App->mainMenu, this, mapFadeTime);
	}

	//Activates collider debug draw mode
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		App->collision->debug = !App->collision->debug;
		App->entities->debug = !App->entities->debug;

	}

	//Activates player's god mode
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		App->entities->player->GodMode();

	//Un-caps the framerate
	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
		App->frameCap = !App->frameCap;

	//Changes the Window Title Display
	if (App->input->GetKey(SDL_SCANCODE_F12) == KEY_DOWN)
		App->windowTitleControl = !App->windowTitleControl;

	//Adds a life
	if (App->input->GetKey(SDL_SCANCODE_KP_1) == KEY_DOWN)
	{
		if (lifes < 99)
			lifes += 1;
	}

	//Minus 1 life
	if (App->input->GetKey(SDL_SCANCODE_KP_2) == KEY_DOWN)
		App->entities->player->LoseALife();

	//Adds Score
	if (App->input->GetKey(SDL_SCANCODE_KP_4) == KEY_DOWN)
		score += 1000;

	//Retrieves Score
	if (App->input->GetKey(SDL_SCANCODE_KP_5) == KEY_DOWN)
		score -= 1000;

	//Adds Coins
	if (App->input->GetKey(SDL_SCANCODE_KP_7) == KEY_DOWN)
		coins += 1;

	//Retrieves Coins
	if (App->input->GetKey(SDL_SCANCODE_KP_8) == KEY_DOWN)
		coins -= 1;

	//Sets Pause in the Game & opens the menu
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		App->pause = !App->pause;
		MenusLoad(UIFunction::FNC_PAUSE);
		App->audio->PlayFx(click.id);
	}

	//Turns volume up
	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN && (App->audio->musicVolume < 100 && App->audio->fxVolume < 100))
	{
		App->audio->musicVolume += 5;
		App->audio->fxVolume += 5;
	}

	//Turns volume down
	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN && (App->audio->musicVolume > 0 && App->audio->fxVolume > 0))
	{
		App->audio->musicVolume -= 5;
		App->audio->fxVolume -= 5;
	}

	//Opens / Closes the Console
	if (App->input->GetKey(SDL_SCANCODE_GRAVE) == KEY_DOWN)
	{
		console->SetToDisable(!console->enabled);
	}

	UIInGameUpdate();

	//Draws the current map
	App->map->Draw();

	// Debug pathfinding w/ mouse ------------------------------
	//Blits the debug pathfinding, if exists
	blitDebugPath();

	//update texture of the time
	UITimeUpdate();




	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	BROFILER_CATEGORY("Scene Post-Update", Profiler::Color::Orange)

		bool ret = true;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	//We make sure all the pointers are disabled----
	debug_tex = nullptr;
	debugPath.Clear();
	App->collision->debug = false;
	App->entities->debug = false;
	App->ui->focused.lookAt = nullptr;

	console = nullptr;
	parent = nullptr;

	//Delete Player
	App->entities->DeletePlayer();
	App->entities->player = nullptr;

	//Delete the UI of the game
	App->ui->DeleteAllElements();

	//Unload All the Game Related SFX
	App->audio->UnLoadAllFx();

	//Reset the Camera
	ResetCamera();

	//Set the gameplay vars to 0 ------------------
	lifes = 0u;
	score = 0u;
	coins = 0u;

	return true;
}

//If the map is different that the one we want to load, we change maps
bool j1Scene::Load(pugi::xml_node& load)
{
	p2SString currentmap = App->map->data.currentmap.GetString();
	if (currentmap != load.child("current_map").attribute("name").as_string())
	{
		App->map->CleanUp();
		App->scene->Reset(load.child("current_map").attribute("name").as_string());
	}

	//Load the gameplay related vars-------------
	lifes = load.child("current_lifes").attribute("value").as_uint();
	score = load.child("current_score").attribute("value").as_uint();
	coins = load.child("current_coins").attribute("value").as_uint();
	time.StartFrom(load.child("current_time").attribute("value").as_int());

	if (App->pause)
	{
		App->pause = false;
		MenusLoad(UIFunction::FNC_PAUSE);
	}
	return true;
}

//Save
bool j1Scene::Save(pugi::xml_node& save) const
{
	save.append_child("current_map").append_attribute("name") = App->map->data.currentmap.GetString(); //Saves the current map info

	//Save of the gameplay related vars-------------
	save.append_child("current_lifes").append_attribute("value") = lifes;
	save.append_child("current_score").append_attribute("value") = score;
	save.append_child("current_coins").append_attribute("value") = coins;

	//Time load------------------
	uint curr_time = time.Read();
	save.append_child("current_time").append_attribute("value") = curr_time;
	return true;
}

//Sets the debug pathfinding w/ the mouse
void j1Scene::setDebugPathfinding()
{
	//We init the vars we will need
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	//We create the path only if we are in debug mode
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && App->entities->debug)
	{
		if (origin_selected == true)
		{
			debugPath.Clear();
			App->pathfinding->CreatePath(origin, p);

			uint pathCount = App->pathfinding->GetLastPath()->Count();

			for (uint i = 0; i < pathCount; i++)
			{
				debugPath.PushBack(*App->pathfinding->GetLastPath()->At(i));
			}
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}

	//Deletes the path if right click is pressed
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{
		origin_selected = false;
		debugPath.Clear();
	}

}

//Blits the debug pathfinding, if exists
void j1Scene::blitDebugPath() const
{
	if (App->entities->debug)
	{
		int x, y;
		App->input->GetMousePosition(x, y);
		iPoint p = App->render->ScreenToWorld(x, y);
		p = App->map->WorldToMap(p.x, p.y);
		p = App->map->MapToWorld(p.x, p.y);

		App->render->Blit(debug_tex, p.x, p.y);

		if (debugPath.Count() > 0)
			for (uint i = 0; i < debugPath.Count(); ++i)
			{
				iPoint pos = App->map->MapToWorld(debugPath.At(i)->x, debugPath.At(i)->y);
				App->render->Blit(debug_tex, pos.x, pos.y);
			}
	}
}

//Camera Logic
void j1Scene::Camera()
{
	//Get the current player position
	fPoint playerPos = App->entities->player->getPos();

	//Calculate the camera acording to the player in both axis
	cameraPos.x = (-(playerPos.x) + width / 2);
	cameraPos.y = (-(playerPos.y) + height / 2);

	//Recalculate it taking account the previous camera
	cameraPos.x += (cameraPos.x * App->win->GetScale() - App->render->camera.w) / 10;
	cameraPos.y += ((cameraPos.y * App->win->GetScale() - App->render->camera.y) / 10);


	//We lock the camera if we get to the edges
	CheckCameraLimits();


	//And then we give the new camera to the render
	App->render->camera.x = cameraPos.x;
	App->render->camera.y = cameraPos.y;

}

//Camera checks in all axis
void j1Scene::CheckCameraLimits()
{
	//Right
	if (cameraPos.x - width < -Hlimit.x)
		cameraPos.x = (-Hlimit.x + ((int)width));

	//Left
	if (cameraPos.x > 0)
		cameraPos.x = 0;

	//Top
	if (cameraPos.y - height < -Hlimit.y)
		cameraPos.y = (-Hlimit.y + ((int)height));

	//Down
	if (cameraPos.y > 0)
		cameraPos.y = 0;
}

//Manages the UI inputs of this module
void j1Scene::OnGui(UIEventType type, UIFunction func, j1UIelement* userPointer, const char* bufferText)
{
	switch (type)
	{
	case UIEventType::EVENT_UPCLICK:
	{

		switch (func)
		{
		case UIFunction::FNC_PAUSE:
		{
			App->pause = !App->pause;
			MenusLoad(UIFunction::FNC_PAUSE);
		}
		break;

		case UIFunction::FNC_OPTIONS:
		{
			MenusLoad(UIFunction::FNC_OPTIONS);
		}
		break;

		case UIFunction::FNC_EXIT:
		{
			App->pause = false;
			App->fade->FadeToBlackMod(App->mainMenu, this, mapFadeTime);
		}
		break;

		case UIFunction::FNC_GOBACK:
		{
			MenusLoad(lastcall);
		}
		break;

		case UIFunction::FNC_SAVE:
		{
			App->SaveGame();
		}
		break;

		case UIFunction::FNC_CONTINUEGAME:
		{
			App->LoadGame();
		}
		break;
		}

		App->audio->PlayFx(click.id);
	}
	break;

	case UIEventType::EVENT_DRAG:
	{
		switch (func)
		{
		case UIFunction::FNC_CHANGE_VMUSIC:
		{
			if (userPointer)
			{
				App->audio->musicVolume = userPointer->GetAudioValue();
			}
		}
		break;

		case UIFunction::FNC_CHANGE_VFX:
		{
			if (userPointer)
			{
				App->audio->fxVolume = userPointer->GetAudioValue();
			}
		}
		break;
		}
	}
	break;

	case UIEventType::EVENT_CONSOLE:
	{
		switch (func)
		{
		case UIFunction::FNC_GODMODE:
		{
			if (App->entities->player)
			{
				App->entities->player->GodMode();
			}
		}
		break;

		case UIFunction::FNC_LOADMAP:
		{
			//We check if the map exists we introduced exists, if a positve case we load it's name
			bufferText = App->map->MapExist(bufferText);
			if (strlen(bufferText) > 2)
			{
				App->fade->FadeToBlackMap(bufferText, -1, App->scene->mapFadeTime);
				LOG("Loading: %s", bufferText);
			}
			else
				LOG("Map Name was Incorrect!", bufferText);

		}
		break;

		case UIFunction::FNC_FPS:
		{
			uint newCap = atoi(bufferText);
			
			//Check Limits
			if (newCap > 120)newCap = 120;
			else if (newCap < 30) newCap = 30;

			App->UpdateFrameCap(newCap);

			LOG("FrameCap is now %u", newCap);
		}
		break;
		}
		break;

	}
	}
}

void j1Scene::UIInGameUpdate()
{
	App->tex->UnLoad(ui_lifes->texture);
	ui_lifes->texture = nullptr;
	sprintf_s(lifes_text, 10, "%02d", lifes);
	ui_lifes->texture = App->fonts->Print(lifes_text);

	App->tex->UnLoad(ui_coins->texture);
	ui_coins->texture = nullptr;
	sprintf_s(coins_text, 10, "%02d", coins);
	ui_coins->texture = App->fonts->Print(coins_text);

	App->tex->UnLoad(ui_score->texture);
	ui_score->texture = nullptr;
	sprintf_s(score_text, 10, "%05d", score);
	ui_score->texture = App->fonts->Print(score_text);
	App->fonts->CalcSize(score_text, ui_score->rect.w, ui_score->rect.h);
}

void j1Scene::UITimeUpdate()
{
	float seconds = time.ReadSec();

	App->tex->UnLoad(ui_time->texture);
	ui_time->texture = nullptr;
	sprintf_s(time_text, "%3.2f", seconds);
	ui_time->texture = App->fonts->Print(time_text);
	App->fonts->CalcSize(time_text, ui_time->rect.w, ui_time->rect.h);
}

void j1Scene::MenusLoad(UIFunction func)
{
	App->ui->ToDeleteElement();

	switch (func)
	{
	case UIFunction::FNC_PAUSE:
		if (App->pause)
		{
			App->ui->AddElement(ui_type::UI_IMAGE, parent, { -260, -85 }, false, false, true, { 1265,36,177,204 })->to_delete = true;
			App->ui->AddElement(ui_type::UI_IMAGE, parent, { -365, -190 }, false, false, true, { 969,34,276,490 })->to_delete = true;
			App->ui->AddElement(ui_type::UI_BUTTON, parent, { -375,-200 }, true, false, true, { 73,992,256,64 }, this, UIFunction::FNC_PAUSE, drag_axis::MOV_NONE, "CONTINUE")->to_delete = true;
			App->ui->AddElement(ui_type::UI_BUTTON, parent, { -375,-300 }, true, false, true, { 73,992,256,64 }, this, UIFunction::FNC_OPTIONS, drag_axis::MOV_NONE, "SETTINGS")->to_delete = true;
			App->ui->AddElement(ui_type::UI_BUTTON, parent, { -375,-400 }, true, false, true, { 73,992,256,64 }, this, UIFunction::FNC_CONTINUEGAME, drag_axis::MOV_NONE, "LOAD")->to_delete = true;
			App->ui->AddElement(ui_type::UI_BUTTON, parent, { -375,-500 }, true, false, true, { 73,992,256,64 }, this, UIFunction::FNC_SAVE, drag_axis::MOV_NONE, "SAVE")->to_delete = true;
			App->ui->AddElement(ui_type::UI_BUTTON, parent, { -375,-600 }, true, false, true, { 73,992,256,64 }, this, UIFunction::FNC_EXIT, drag_axis::MOV_NONE, "MAIN MENU")->to_delete = true;
			time.Stop();
		}
		else
			time.ReStart();
		break;

	case UIFunction::FNC_OPTIONS:

		App->ui->AddElement(ui_type::UI_IMAGE, parent, { -295,-290 }, false, false, true, { 969,763,400,306 })->to_delete = true;
		App->ui->AddElement(ui_type::UI_TEXT, parent, { -375,-370 }, false, false, true, { 0,0,0,0 }, nullptr, UIFunction::FNC_NONE, drag_axis::MOV_NONE, "Music Volume:")->to_delete = true;
		App->ui->AddElement(ui_type::UI_SLIDER, parent, { -375,-400 }, true, false, true, { 0,0,0,0 }, this, UIFunction::FNC_CHANGE_VMUSIC, drag_axis::MOV_X)->to_delete = true;
		App->ui->AddElement(ui_type::UI_TEXT, parent, { -375,-470 }, false, false, true, { 0,0,0,0 }, nullptr, UIFunction::FNC_NONE, drag_axis::MOV_NONE, "Fx Volume:")->to_delete = true;
		App->ui->AddElement(ui_type::UI_SLIDER, parent, { -375,-500 }, true, false, true, { 0,0,0,0 }, this, UIFunction::FNC_CHANGE_VFX, drag_axis::MOV_X)->to_delete = true;
		App->ui->AddElement(ui_type::UI_BUTTON, parent, { -305,-300 }, true, false, true, { 221,554,64,64 }, this, UIFunction::FNC_GOBACK, drag_axis::MOV_NONE)->to_delete = true;

		lastcall = UIFunction::FNC_PAUSE;

		break;
	}

}

//Resets the Camera
void j1Scene::ResetCamera()
{
	App->render->camera.x = 0;
	App->render->camera.y = 0;
}


//Checks if the maximum score has been surpassed
bool j1Scene::CheckMaxScore()
{
	bool ret = false;
	if (score > maxScore)
	{
		//Save Score-----------------------------------
		LOG("Saving Game new Max Score");
		ret = true;

		pugi::xml_node max_score = sceneConfig.child("maxScore");
		max_score.attribute("value").set_value(score);
		maxScore = score;

		App->saveConfigFile();
	}


	return ret;
}
