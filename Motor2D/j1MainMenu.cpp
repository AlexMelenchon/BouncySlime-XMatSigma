#include "j1MainMenu.h"
#include "j1UIManager.h"
#include "j1FadeToBlack.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Input.h"

//Constructor
j1MainMenu::j1MainMenu()
{}

// Destructor
j1MainMenu::~j1MainMenu()
{}


// Called before render is available
bool j1MainMenu::Awake(pugi::xml_node& config)
{
	menuMusic();
	bool ret = true;
	return true;
}

// Called before the first frame
bool j1MainMenu::Start()
{
	bool ret = true;

	//We draw the background scene
	App->map->Load(App->map->menu_tmx.GetString());

	//Start the music
	App->audio->PlayMusic("audio/music/mainmenu_loop.ogg", 0.0f);


	//We create the "ghost" parent
	parent = App->ui->AddElement(ui_type::UI_IMAGE, nullptr, { 0,0 }, false, false, false, { 0,0,0,0 }, this, UIFunction::FNC_NONE, drag_axis::MOV_NONE);

	//We create the UI
	App->ui->AddElement(ui_type::UI_BUTTON, nullptr, { 700,250 }, true, false, true, { 73,992,256,64 }, this, UIFunction::FNC_STARTGAME, drag_axis::MOV_NONE, "Play");
	App->ui->AddElement(ui_type::UI_BUTTON, nullptr, { 700,350 }, true, false, true, { 73,992,256,64 }, this, UIFunction::FNC_CONTINUEGAME, drag_axis::MOV_NONE, "Continue");
	App->ui->AddElement(ui_type::UI_BUTTON, nullptr, { 700,450 }, true, false, true, { 73,992,256,64 }, this, UIFunction::FNC_OPTIONS, drag_axis::MOV_NONE, "Settings");
	App->ui->AddElement(ui_type::UI_BUTTON, nullptr, { 700,550 }, true, false, true, { 73,992,256,64 }, this, UIFunction::FNC_CREDITS, drag_axis::MOV_NONE, "Credits");
	App->ui->AddElement(ui_type::UI_BUTTON, nullptr, { 700,650 }, true, false, true, { 73,992,256,64 }, this, UIFunction::FNC_EXIT, drag_axis::MOV_NONE, "Exit");

	App->ui->AddElement(ui_type::UI_IMAGE, nullptr, { 20,100 }, false, false, true, { 491,1022,394,168 }, this, UIFunction::FNC_NONE, drag_axis::MOV_NONE);

	App->ui->AddElement(ui_type::UI_BUTTON, nullptr, { 20,650 }, true, false, true, {331,991,64,64 }, this, UIFunction::FNC_GITHUB, drag_axis::MOV_NONE);


	lastcall = UIFunction::FNC_NONE;
	

	return true;
}

// Called before all Updates
bool j1MainMenu::PreUpdate()
{
	//Activates the UI debug mode
	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
		App->ui->debug = !App->ui->debug;

	return exiting;
}

// Called each loop iteration
bool j1MainMenu::Update(float dt)
{
	//We draw the background scene
	App->map->Draw();

	return exiting;
}

// Called before all Updates
bool j1MainMenu::PostUpdate()
{
	return exiting;
}

// Called before quitting
bool j1MainMenu::CleanUp()
{
	bool ret = true;

	//Delete all the UI and get ready to start the game
	App->ui->DeleteAllElements();

	//Unload All the main menu related SFX
	App->audio->UnLoadAllFx();

	return true;
}

//Manages the UI inputs of this module
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
			App->fade->FadeToBlackMod(App->scene, this, App->scene->mapFadeTime);
			break;
		}
		case UIFunction::FNC_CONTINUEGAME:
		{
			App->fade->FadeToBlackMod(App->scene, this, App->scene->mapFadeTime, true);
			break;
		}

		case UIFunction::FNC_OPTIONS:
		{
			
			MenusLoad(UIFunction::FNC_OPTIONS);
			break;
		}
		case UIFunction::FNC_CREDITS:

			
			MenusLoad(UIFunction::FNC_CREDITS);

			break;

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

//Loads the options sub-menu
void j1MainMenu::MenusLoad(UIFunction func)
{
		

		switch (func)
		{


		case UIFunction::FNC_OPTIONS:

			App->ui->ToDeleteElement();

			App->ui->AddElement(ui_type::UI_TEXT, parent, { -50,-370 }, false, false, true, { 0,0,0,0 }, nullptr, UIFunction::FNC_NONE, drag_axis::MOV_NONE, "Music Volume:")->to_delete = true;
			App->ui->AddElement(ui_type::UI_SLIDER, parent, { -50,-400 }, true, false, true, { 0,0,0,0 }, this, UIFunction::FNC_CHANGE_VMUSIC, drag_axis::MOV_X)->to_delete = true;
			App->ui->AddElement(ui_type::UI_TEXT, parent, { -50,-470 }, false, false, true, { 0,0,0,0 }, nullptr, UIFunction::FNC_NONE, drag_axis::MOV_NONE, "Fx Volume:")->to_delete = true;
			App->ui->AddElement(ui_type::UI_SLIDER, parent, { -50,-500 }, true, false, true, { 0,0,0,0 }, this, UIFunction::FNC_CHANGE_VFX, drag_axis::MOV_X)->to_delete = true;

			break;

		case UIFunction::FNC_CREDITS:

			App->ui->ToDeleteElement();

			App->ui->AddElement(ui_type::UI_IMAGE, parent, { -20,-370 }, false, false, true, { 45,1436,600,210 }, nullptr)->to_delete = true;
			App->ui->AddElement(ui_type::UI_IMAGE, parent, { -25,-390 }, false, false, true, {64,1244,590,185 }, nullptr)->to_delete = true;
			App->ui->AddElement(ui_type::UI_IMAGE, parent, { -105,-580 }, false, false, true, { 717,1230,122,120 }, nullptr)->to_delete = true;

			break;

		}

		if (lastcall == func)
		{
			App->ui->ToDeleteElement();
			lastcall = UIFunction::FNC_NONE;
		}
		else
			lastcall = func;

}
