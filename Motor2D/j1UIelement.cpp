#include "j1UIelement.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1UIManager.h"
#include "j1Collision.h"

j1UIelement::~j1UIelement()
{
}

bool j1UIelement::Start()
{
	text = App->ui->GetAtlas();
	return true;
}

void j1UIelement::Draw()
{
	App->render->Blit(text, globalPos.x + localPos.x, globalPos.y + localPos.y, NULL);
}

bool j1UIelement::OnHover()
{	
	
	bool ret = false;
	SDL_Point mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);

	if (SDL_PointInRect(&mouse, &this->rect) && this->enabled && this->interact)
		ret = true;
	

	return ret;	
}

void j1UIelement::OnClick()
{

	
}

void j1UIelement::OnRelease()
{

}




