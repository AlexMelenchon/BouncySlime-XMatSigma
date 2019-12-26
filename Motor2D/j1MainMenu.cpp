#include "j1MainMenu.h"
#include "j1UIManager.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"

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

	//App->ui->AddElement(ui_type::UI_BUTTON, App->ui->AddElement(ui_type::UI_BUTTON, nullptr, { 100,500 }, true, true, true, { 73,406,64,64 }, this)
	//	, { 100,300 }, true, true, true, { 73,406,64,64 }, this, UIFunction::FNC_PAUSE);

	//App->ui->AddElement(ui_type::UI_INPUTBOX, nullptr, { 100,600 }, true, true, true, { 0,0,0,0 }, this, UIFunction::FNC_NONE, drag_axis::MOV_Y, "macarró");
	//App->ui->AddElement(ui_type::UI_SLIDER, nullptr, { 100,400 }, true, true, true, { 0,0,0,0 }, this, UIFunction::FNC_NONE, drag_axis::MOV_X);

	App->ui->AddElement(ui_type::UI_BUTTON, nullptr, { 100,100 }, true, false, true, { 73,992,256,64 }, this, UIFunction::FNC_CONTINUEGAME, drag_axis::MOV_NONE, "Start Game");



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