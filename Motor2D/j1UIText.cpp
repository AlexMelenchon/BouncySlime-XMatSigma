#include "j1UIButton.h"
#include "j1Scene.h"
#include "j1Input.h"
#include "j1UIText.h"
#include "j1Render.h"
#include "j1Fonts.h"

j1UIText::j1UIText()
{
	this->type = ui_type::UI_IMAGE;
}
j1UIText::j1UIText(const char* txt)
{
	this->type = ui_type::UI_IMAGE;
	this->texture =	App->fonts->Print(txt);
	App->fonts->CalcSize(txt, w, h);
}

j1UIText::~j1UIText()
{}

bool j1UIText::Awake(pugi::xml_node&)
{
	return true;
}

bool j1UIText::Start()
{

	return true;
}

bool j1UIText::PreUpdate()
{
	return true;
}

bool j1UIText::Update(float dt)
{
	return true;
}

bool j1UIText::PostUpdate(bool debug)
{
	Draw(debug);
	return true;
}

bool j1UIText::CleanUp()
{
	return true;
}

void j1UIText::Draw(bool debug)
{
	rect.w = w;
	rect.h = h;

	App->render->Blit(texture, Position.x, Position.y, &rect, 0.0f);

	if (debug)
	{
		App->render->DrawQuad({ Position.x, Position.y, rect.w, rect.h }, 0, 255, 255, 255, false, false);
	}

}
