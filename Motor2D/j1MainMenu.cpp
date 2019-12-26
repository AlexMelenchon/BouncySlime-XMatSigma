#include "j1MainMenu.h"
#include "j1UIManager.h"
#include "j1FadeToBlack.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Input.h"

j1MainMenu::j1MainMenu()
{}


j1MainMenu::~j1MainMenu()
{}

bool j1MainMenu::Awake(pugi::xml_node& config)
{
	bool ret = true;

	

	return true;
}

bool j1MainMenu::Start()
{
	bool ret = true;

	App->map->Load(App->map->menu_tmx.GetString());


	parent = App->ui->AddElement(ui_type::UI_IMAGE, nullptr, { 0,0 }, false, false, false, { 0,0,0,0 }, this, UIFunction::FNC_NONE, drag_axis::MOV_NONE);

	App->ui->AddElement(ui_type::UI_BUTTON, nullptr, { 700,250 }, true, false, true, { 73,992,256,64 }, this, UIFunction::FNC_STARTGAME, drag_axis::MOV_NONE, "Play");
	App->ui->AddElement(ui_type::UI_BUTTON, nullptr, { 700,350 }, true, false, true, { 73,992,256,64 }, this, UIFunction::FNC_CONTINUEGAME, drag_axis::MOV_NONE, "Continue");
	App->ui->AddElement(ui_type::UI_BUTTON, nullptr, { 700,450 }, true, false, true, { 73,992,256,64 }, this, UIFunction::FNC_OPTIONS, drag_axis::MOV_NONE, "Settings");
	App->ui->AddElement(ui_type::UI_BUTTON, nullptr, { 700,550 }, true, false, true, { 73,992,256,64 }, this, UIFunction::FNC_CREDITS, drag_axis::MOV_NONE, "Credits");
	App->ui->AddElement(ui_type::UI_BUTTON, nullptr, { 700,650 }, true, false, true, { 73,992,256,64 }, this, UIFunction::FNC_EXIT, drag_axis::MOV_NONE, "Exit");

	App->ui->AddElement(ui_type::UI_BUTTON, nullptr, { 20,650 }, true, false, true, {331,991,64,64 }, this, UIFunction::FNC_GITHUB, drag_axis::MOV_NONE);

	
	return true;
}

bool j1MainMenu::PreUpdate()
{
	//Loads the game
	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
		App->ui->debug = !App->ui->debug;

	return exiting;
}

bool j1MainMenu::Update(float dt)
{
	App->map->Draw();

	return exiting;
}

bool j1MainMenu::PostUpdate()
{
	return exiting;
}

bool j1MainMenu::CleanUp()
{
	bool ret = true;

	//Delete all the UI and get ready to start the game
	App->ui->DeleteAllElements();

	//Unload All the main menu related SFX
	App->audio->UnLoadAllFx();

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

		case UIFunction::FNC_GITHUB:
			ShellExecuteA(NULL, "open", "https://github.com/AlexMelenchon/BouncySlime-XMatSigma", NULL, NULL, SW_SHOWNORMAL);
			break;
		case UIFunction::FNC_EXIT:
		{
			exiting = false;
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
		App->ui->AddElement(ui_type::UI_TEXT, parent, { -200,-170 }, false, false, true, { 0,0,0,0 }, nullptr, UIFunction::FNC_NONE, drag_axis::MOV_NONE, "Music Volume:")->to_delete = true;
		App->ui->AddElement(ui_type::UI_SLIDER, parent, { -200,-200 }, true, false, true, { 0,0,0,0 }, this, UIFunction::FNC_CHANGE_VMUSIC, drag_axis::MOV_X)->to_delete = true;
		App->ui->AddElement(ui_type::UI_TEXT, parent, { -200,-270 }, false, false, true, { 0,0,0,0 }, nullptr, UIFunction::FNC_NONE, drag_axis::MOV_NONE, "Fx Volume:")->to_delete = true;
		App->ui->AddElement(ui_type::UI_SLIDER, parent, { -200,-300 },true, false, true, { 0,0,0,0 }, this, UIFunction::FNC_CHANGE_VFX, drag_axis::MOV_X)->to_delete = true;
	}
	
	else
	{
		//Check if though all the list if they have the flag to delete = true;
		//Call DeleteElement to them

		App->ui->ToDeleteElement();
	}
}