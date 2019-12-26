#include "j1MainMenu.h"
#include "j1UIManager.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"
#include "j1Audio.h"

j1MainMenu::j1MainMenu()
{}


j1MainMenu::~j1MainMenu()
{}

bool j1MainMenu::Awake(pugi::xml_node&)
{
	bool ret = true;

	return true;
}

bool j1MainMenu::Start()
{
	bool ret = true;


	return true;
}

bool j1MainMenu::PreUpdate()
{
	bool ret = true;

	return true;
}

bool j1MainMenu::Update(float dt)
{
	bool ret = true;

	return true;
}

bool j1MainMenu::PostUpdate()
{
	bool ret = true;

	return true;
}

bool j1MainMenu::CleanUp()
{
	bool ret = true;
	App->ui->CleanUp();

	return true;
}

void j1MainMenu::OnGui(UIEventType type, UIFunction func, j1UIelement* userPointer)
{
	switch (type)
	{
	case UIEventType::EVENT_UPCLICK:
	{
		switch (func)
		{
		case UIFunction::FNC_STARTGAME:
		{
			App->fade->FadeToBlack(App->scene, this, App->scene->mapFadeTime);
			break;
		}
		case UIFunction::FNC_CONTINUEGAME:
		{
			App->fade->FadeToBlack(App->scene, this, App->scene->mapFadeTime, true);
			break;
		}

		case UIFunction::FNC_OPTIONS:
		{
			optionsMenu = !optionsMenu;
			OptionsLoad(optionsMenu);
			break;
		}

		}
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
			break;
		}

		case UIFunction::FNC_CHANGE_VFX:
		{
			if (userPointer)
			{
				App->audio->fxVolume = userPointer->GetAudioValue();
			}
			break;
		}

		}
	}
	break;
	}



}

void j1MainMenu::OptionsLoad(bool to_load)
{
	if (to_load)
	{
		//TODO: Create the menu
		//Ex. AddElement(...)->to_delete = true;

	}
	else
	{
		//Check if though all the list if they have the flag to delete = true;
		//Call DeleteElement to them


	}


}