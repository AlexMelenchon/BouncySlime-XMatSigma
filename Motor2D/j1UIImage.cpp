#include "j1UIImage.h"
#include "j1Render.h"
#include "j1UIManager.h"

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
	if (parent && parent->type == ui_type::UI_INPUTBOX && rect.x == -1)
	{
		this->texture = nullptr;
	}
	else
		texture = App->ui->GetAtlas();


	return true;
}


// Called before all Updates
bool j1UIImage::PostUpdate(bool debug)
{
	if (texture)
		Draw(debug);
	else
		NoTextDraw(debug);
	return true;
}

void j1UIImage::NoTextDraw(bool debug)
{
	//Blit the element into screen
	App->render->DrawQuad({ Position.x, Position.y, rect.w, rect.h }, 0, 0, 0, 255, true, false);

	//In debug mode, we also drag its rectangle
	if (debug)
	{
		App->render->DrawQuad({ Position.x, Position.y, rect.w, rect.h }, 0, 255, 255, 255, false, false);
	}
}
