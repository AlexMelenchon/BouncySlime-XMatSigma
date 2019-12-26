#include "j1MainMenu.h"
#include "j1UIManager.h"
#include "j1FadeToBlack.h"
#include "j1Map.h"
#include "j1Scene.h"

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

 bool p = App->map->Load(App->map->menu_tmx.GetString());

	App->ui->AddElement(ui_type::UI_BUTTON, nullptr, { 700,250 }, true, false, true, { 73,992,256,64 }, this, UIFunction::FNC_CONTINUEGAME, drag_axis::MOV_NONE, "Play");
	App->ui->AddElement(ui_type::UI_BUTTON, nullptr, { 700,350 }, true, false, true, { 73,992,256,64 }, this, UIFunction::FNC_CONTINUEGAME, drag_axis::MOV_NONE, "Continue");
	App->ui->AddElement(ui_type::UI_BUTTON, nullptr, { 700,450 }, true, false, true, { 73,992,256,64 }, this, UIFunction::FNC_OPTIONS, drag_axis::MOV_NONE, "Settings");
	App->ui->AddElement(ui_type::UI_BUTTON, nullptr, { 700,550 }, true, false, true, { 73,992,256,64 }, this, UIFunction::FNC_CREDITS, drag_axis::MOV_NONE, "Credits");
	App->ui->AddElement(ui_type::UI_BUTTON, nullptr, { 700,650 }, true, false, true, { 73,992,256,64 }, this, UIFunction::FNC_EXIT, drag_axis::MOV_NONE, "Exit");

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

	App->map->Draw();

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

void j1MainMenu::OnGui(UIEventType type, UIFunction func)
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
			App->fade->FadeToBlack(App->scene, this, App->scene->mapFadeTime,true);
			break;
		}

		}
	}
	break;

	}



}