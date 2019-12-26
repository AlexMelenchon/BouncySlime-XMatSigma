#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1Input.h"

#include "SDL/include/SDL.h"

//Constructor
j1Window::j1Window() : j1Module()
{
	window = NULL;
	screen_surface = NULL;
	name.create("window");
}

// Destructor
j1Window::~j1Window()
{}

// Called before render is available
bool j1Window::Awake(pugi::xml_node& config)
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		Uint32 flags = SDL_WINDOW_SHOWN;
		bool fullscreen = config.child("fullscreen").attribute("value").as_bool(false);
		bool borderless = config.child("borderless").attribute("value").as_bool(false);
		bool resizable = config.child("resizable").attribute("value").as_bool(false);
		bool fullscreen_window = config.child("fullscreen_window").attribute("value").as_bool(false);

		width = config.child("resolution").attribute("width").as_int(640);
		height = config.child("resolution").attribute("height").as_int(480);
		scale = config.child("resolution").attribute("scale").as_int(1);
		if(fullscreen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(borderless == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(resizable == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(fullscreen_window == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(App->GetTitle(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called at the end of the frame
bool j1Window::PostUpdate()
{
	bool ret = true;

	ShowWindowTitle();

	return ret;
}

// Called before quitting
bool j1Window::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

// Set new window title
void j1Window::SetTitle(const char* new_title)
{
	//title.create(new_title);
	SDL_SetWindowTitle(window, new_title);
}

// Retrive window size
void j1Window::GetWindowSize(uint& width, uint& height) const
{
	width = this->width;
	height = this->height;
}

// Retrieve window scale
uint j1Window::GetScale() const
{
	return scale;
}


//Sets the window title for the map info
void j1Window::ShowWindowTitle() const
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