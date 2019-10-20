#include <math.h>
#include "j1Render.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Player.h"
#include "j1Collision.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"

j1FadeToBlack::j1FadeToBlack()
{ }

bool j1FadeToBlack::Awake(pugi::xml_node& config)
{
	name.create("fade");

	screen = { 0, 0, (int)App->win->width * (int)App->win->GetScale(), (int)App->win->height * (int)App->win->GetScale() };

	return true;
}

j1FadeToBlack::~j1FadeToBlack()
{}

// Load assets
bool j1FadeToBlack::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

// Update: draw background
bool j1FadeToBlack::PostUpdate()
{
	bool ret = true;

	if (current_step == fade_step::none)
		return true;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch(current_step)
	{
		case fade_step::fade_to_black:
		{
			if(now >= total_time)
			{
				App->map->CleanUp();
				App->scene->Reset(mapToLoad);
				App->input->Disable();
				// ---
				total_time += total_time;
				start_time = SDL_GetTicks();
				current_step = fade_step::fade_from_black;
			}
		} break;

		case fade_step::fade_from_black:
		{
			normalized = 1.0f - normalized;

			if(now >= total_time)
			{
				current_step = fade_step::none;
				App->input->Enable();
				App->player->control_death = false;
			}
		} break;
	}

	// Finally render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return ret;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool j1FadeToBlack::FadeToBlack(const char* mapName, float time)
{
	bool ret = false;


	if(current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		mapToLoad = mapName;
		ret = true;
	}

	return ret;
}