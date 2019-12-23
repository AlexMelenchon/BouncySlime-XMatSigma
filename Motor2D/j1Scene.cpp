#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "j1FadeToBlack.h"
#include "j1Collision.h"
#include "j1EntityManager.h"
#include "j1UIManager.h"


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

	mapFadeTime = scene_config.child("mapFadeTime").text().as_float();


	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	//Loads the first map
	Reset(App->map->data.maplist.start->data->name.GetString());

	debug_tex = App->entities->debug_tex;

	App->ui->AddElement(ui_type::UI_BUTTON, App->ui->AddElement(ui_type::UI_BUTTON, nullptr, { 100,500 }, true, false, true, { 73,406,64,64 }, this)
		, { 100,300 }, true, true, true, { 73,406,64,64 }, this, UIFunction::FNC_PAUSE);


	return true;
}

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
		App->fade->FadeToBlack(App->map->data.maplist.start->data->name.GetString(), NULL, mapFadeTime);

	//Loads the 2nd map
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		App->fade->FadeToBlack(App->map->data.maplist.At(1)->data->name.GetString(), NULL, mapFadeTime);

	//Loads the 3rd map
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		App->fade->FadeToBlack(App->map->data.maplist.At(2)->data->name.GetString(), NULL, mapFadeTime);

	//Reloads current map (a.k.a player's death)
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
		App->fade->FadeToBlack(App->map->data.currentmap.GetString(), NULL, mapFadeTime);

	//Saves the game
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	//Loads the game
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	//Loads the game
	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
		App->ui->debug = !App->ui->debug;

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
	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
		App->windowTitleControl = !App->windowTitleControl;

	//Sets Pause in the Game
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		App->pause = !App->pause;
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


	//Draws the current map
	App->map->Draw();

	//Sets the window title for the map info
	showWindowTitle();


	// Debug pathfinding w/ mouse ------------------------------
	//Blits the debug pathfinding, if exists
	blitDebugPath();



	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{

	BROFILER_CATEGORY("Scene Post-Update", Profiler::Color::Orange)

		bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

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
	return true;
}

//Save
bool j1Scene::Save(pugi::xml_node& save) const
{
	save.append_child("current_map").append_attribute("name") = App->map->data.currentmap.GetString(); //Saves the current map info
	return true;
}

//Sets the window title for the map info
void j1Scene::showWindowTitle() const
{
	if (App->windowTitleControl) //Map related info
	{
		p2SString title("%s - %s || Map:%dx%d Tiles:%dx%d Tilesets:%d Mouse Position X:%d Y:%d Mouse Tilset:%d,%d Current Map:%s",
			App->GetTitle(), App->GetOrganization(),
			App->map->data.width, App->map->data.height,
			App->map->data.tile_width, App->map->data.tile_height,
			App->map->data.tilesets.count(), App->input->mouse_x - App->render->camera.x,
			App->input->mouse_y - App->render->camera.y,
			(App->input->mouse_x - App->render->camera.x) / App->map->data.tile_width,
			(App->input->mouse_y - App->render->camera.y) / App->map->data.tile_height,
			App->map->data.currentmap.GetString());

		App->win->SetTitle(title.GetString());
	}
	else //Frame control info
	{
		p2SString cap;
		if (App->frameCap)
			cap.create("ON");
		else
			cap.create("OFF");


		p2SString title("%s - %s || FPS: %i Av.FPS: %.2f || FrameCap: %s FrameLimit: %i VSYNC %s || Last Frame Ms: %u ",
			App->GetTitle(), App->GetOrganization(),
			App->frames_on_last_update, App->avg_fps,
			cap.GetString(), App->capTime,
			App->render->vsync.GetString(),
			App->last_frame_ms);

		App->win->SetTitle(title.GetString());
	}
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
void j1Scene::OnGui(UIEventType type, UIFunction func)
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
			break;
		}
		}


		break;
	}



	}



}
