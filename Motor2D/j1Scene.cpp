#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "j1FadeToBlack.h"
#include "j1Collision.h"

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

	return true;
}

bool j1Scene::Reset(const char* map)
{
	//Gets the current window width & height
	App->win->GetWindowSize(width, height);

	//Loads the map
	App->map->Load(map);

	// Limit for the end of the map
	Hlimit.x = App->map->data.tile_width * App->map->data.width; 
	Hlimit.y = App->map->data.tile_height * App->map->data.height;

	//Start the music
	App->audio->PlayMusic(App->map->data.music.GetString(),0.0f);

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	//Camera logic
	Camera();
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	//--------DEBUG---------//

	//Loads the 1st map
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		App->fade->FadeToBlack(App->map->data.maplist.start->data->name.GetString(),NULL , mapFadeTime);
	
	//Loads the 2nd map
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		App->fade->FadeToBlack(App->map->data.maplist.At(1)->data->name.GetString(), NULL ,mapFadeTime);

	//Loads the 3rd map
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		App->fade->FadeToBlack(App->map->data.maplist.At(2)->data->name.GetString(), NULL ,  mapFadeTime);

	//Reloads current map (a.k.a player's death)
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
		App->fade->FadeToBlack(App->map->data.currentmap.GetString(), NULL , mapFadeTime);

	//Saves the game
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	//Loads the game
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	//Activates collider debug draw mode
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		App->collision->debug = !App->collision->debug;

	//Activates player's god mode
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		App->player->GodMode();

	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
		App->windowControl = !App->windowControl;

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

	if (App->windowControl)
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

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
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

//Camera Logic
void j1Scene::Camera()
{
	//Get the current player position
	fPoint playerPos = App->player->getPos();

	//Calculate the camera acording to the player in both axis
	cameraPos.x = (-(playerPos.x) + width / 2);
	cameraPos.y = (-(playerPos.y) + height / 2);

	//Recalculate it taking account the previous camera
	cameraPos.x += (cameraPos.x * App->win->GetScale() - App->render->camera.w) / 10;
	cameraPos.y += ((cameraPos.y * App->win->GetScale() - App->render->camera.y )/10);

	
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


