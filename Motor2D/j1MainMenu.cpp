#include "j1MainMenu.h"
#include "j1UIManager.h"

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

	App->ui->AddElement(ui_type::UI_BUTTON, App->ui->AddElement(ui_type::UI_BUTTON, nullptr, { 100,500 }, true, true, true, { 73,406,64,64 }, this)
	, { 100,300 }, true, true, true, { 73,406,64,64 }, this, UIFunction::FNC_PAUSE);

	App->ui->AddElement(ui_type::UI_INPUTBOX, nullptr, { 100,600 }, true, true, true, { 0,0,0,0 }, this, UIFunction::FNC_UNKOWNN, drag_axis::MOV_Y, "macarró");
	App->ui->AddElement(ui_type::UI_SLIDER, nullptr, { 100,400 }, true, true, true, { 0,0,0,0 }, this, UIFunction::FNC_UNKOWNN, drag_axis::MOV_X);


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

	return true;
}

void j1MainMenu::OnGui(UIEventType type, UIFunction func)
{}