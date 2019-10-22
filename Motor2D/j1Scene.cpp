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

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->win->GetWindowSize(width, height);
	App->map->Load("map1.tmx");
	Hlimit.x = App->map->data.tile_width * App->map->data.width;
	Hlimit.y = App->map->data.tile_height * App->map->data.height;

	return true;
}

bool j1Scene::Reset(const char* map)
{
	App->win->GetWindowSize(width, height);
	App->map->Load(map);
	Hlimit.x = App->map->data.tile_width * App->map->data.width;
	Hlimit.y = App->map->data.tile_height * App->map->data.height;
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	Camera();
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{

	if(App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		App->LoadGame();

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		App->SaveGame();


	//App->render->Blit(img, 0, 0);
	App->map->Draw();

	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Mouse Position X:%d Y:%d Mouse Tilset:%d,%d Current Map:%s",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count(), App->input->mouse_x - App->render->camera.x,
		App->input->mouse_y - App->render->camera.y,
		(App->input->mouse_x - App->render->camera.x) / App->map->data.tile_width,
		(App->input->mouse_y - App->render->camera.y) / App->map->data.tile_height,
		App->map->data.currentmap.GetString());

	App->win->SetTitle(title.GetString());

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

void j1Scene::CheckCameraLimits()
{
	if (cameraPos.x - width < -Hlimit.x)
		cameraPos.x = (-Hlimit.x + ((int)width));

	if (cameraPos.x > 0)
		cameraPos.x = 0;

	if (cameraPos.y + height > Hlimit.y + 32)
		cameraPos.y = (Hlimit.y+32 - ((int)height));

	if (cameraPos.y < 0)
		cameraPos.y = 0;


}


