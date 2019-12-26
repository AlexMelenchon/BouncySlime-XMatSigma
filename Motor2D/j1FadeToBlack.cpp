#include <math.h>
#include "j1Render.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Player.h"
#include "j1Audio.h"
#include "j1Collision.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"

//Contructor
j1FadeToBlack::j1FadeToBlack()
{}

//Destructor
j1FadeToBlack::~j1FadeToBlack()
{}

// Called before render is available
bool j1FadeToBlack::Awake(pugi::xml_node& config)
{
	name.create("fade");

	screen = { 0, 0, (int)App->win->width * (int)App->win->GetScale(), (int)App->win->height * (int)App->win->GetScale() };

	return true;
}

// Load assets
bool j1FadeToBlack::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

// Called each loop iteration: draw fading background
bool j1FadeToBlack::PostUpdate()
{
	bool ret = true;

	if (current_step == fade_step::none)
		return true;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case fade_step::fade_to_black:
	{
		if (now >= total_time)
		{
			App->map->CleanUp();

			if (moduleOff)
				moduleOff->Disable();

			if (ModuleOn)
				ModuleOn->Enable();

			if (mapToLoad != nullptr)
			{
				App->scene->Reset(mapToLoad);
			}

			App->input->ReSetKeys();
			App->input->Disable();

			if (load)
			{
				App->LoadGame();
				load = false;
			}


			// ---
			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = fade_step::fade_from_black;
		}
	} break;

	case fade_step::fade_from_black:
	{
		normalized = 1.0f - normalized;

		if (now >= total_time)
		{
			current_step = fade_step::none;

			mapToLoad = nullptr;
			ModuleOn = nullptr;
			moduleOff = nullptr;

			App->input->Enable();
		}
	} break;
	}

	// Finally render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return ret;
}




// Fade to black. At mid point deactivate one map, then activate the desired one
bool j1FadeToBlack::FadeToBlack(const char* mapName, int id, float time)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		App->audio->PlayFx(id);
		mapToLoad = mapName;
		ret = true;
	}

	return ret;
}

bool j1FadeToBlack::FadeToBlack(j1Module* SceneIn, j1Module* SceneOut, float time, bool load)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		ModuleOn = SceneIn;
		moduleOff = SceneOut;
		this->load = load;
		ret = true;
	}
	return true;
}