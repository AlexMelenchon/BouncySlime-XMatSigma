#include "j1UIImage.h"
#include "j1Render.h"
#include "j1UIManager.h"
#include "j1Window.h"
#include "j1Fonts.h"
#include "j1Input.h"

//Constructor
j1UIImage::j1UIImage()
{
	this->type = ui_type::UI_IMAGE;
}

//Destructor
j1UIImage::~j1UIImage()
{}

// Called before the first frame when created
bool j1UIImage::Start()
{
	//Flag to indicate that they are part
	if (parent && (parent->type == ui_type::UI_INPUTBOX || parent->type == ui_type::UI_CONSOLE) && rect.x == NULL)
	{
		this->texture = nullptr;
		rect.w = App->win->width;

		switch (parent->type)
		{
		case  ui_type::UI_INPUTBOX:
		{
			noTextColor = { 90,90,90,90 };
			int w, h = 0;
			TTF_SizeText(App->fonts->default_font, "sample", &w, &h);
			rect.h = h;
		}
		break;

		case  ui_type::UI_CONSOLE:
		{
			noTextColor = { 0,0,0,255 };
		}
		break;

		}
	}
	else
		texture = App->ui->GetAtlas();


	return true;
}


// Called before all Updates
bool j1UIImage::PostUpdate(bool debug)
{
	if (!enabled)
		return true;

	if (texture)
		Draw(debug);
	else
		NoTextDraw(debug);

	return true;
}

void j1UIImage::NoTextDraw(bool debug)
{
	//Blit the element into screen
	App->render->DrawQuad({ Position.x, Position.y, rect.w, rect.h }, noTextColor.r, noTextColor.g, noTextColor.b, noTextColor.a, true, false);

	//In debug mode, we also drag its rectangle
	if (debug)
	{
		App->render->DrawQuad({ Position.x, Position.y, rect.w, rect.h }, 0, 255, 255, 255, false, false);
	}
}


bool j1UIImage::DeFocus()
{
	bool ret = true;
	//When the player clicks outside the inputBox, we stop focusing on it
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && !this->hovering)
		App->ui->focused.lookAt = nullptr;

	return ret;
}
