#include "j1UISlider.h"
#include "j1UIManager.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"

//Constructors----
j1UISlider::j1UISlider()
{
	this->type = ui_type::UI_SLIDER;
}

j1UISlider::j1UISlider(drag_axis axis)
{
	this->type = ui_type::UI_SLIDER;
	this->axis = axis;

	//Create the child elements
	background = App->ui->AddElement(ui_type::UI_IMAGE, this, { 0,0 }, false, false, true, { 73,992,256,64 }, this->listener, UIFunction::FNC_NONE, this->axis);
	button = App->ui->AddElement(ui_type::UI_BUTTON, this, { 0,0 }, true, true, true, { 73, 406, 64, 64 }, this->listener, UIFunction::FNC_NONE, this->axis);
}

//Destructors
j1UISlider::~j1UISlider()
{}

// Called before the first frame when created
bool j1UISlider::Start()
{
	if (this->function == UIFunction::FNC_CHANGE_VFX)
	{
		button->PostoParent.x = -((int)App->audio->fxVolume * (background->rect.w - button->rect.w)) / 128;
		button->Position.x = button->parent->Position.x - button->PostoParent.x;
	}

	if (this->function == UIFunction::FNC_CHANGE_VMUSIC)
	{
		button->PostoParent.x = -((int)App->audio->musicVolume * (background->rect.w - button->rect.w)) / 128;
		button->Position.x = button->parent->Position.x - button->PostoParent.x;
	}

	background->listener = this->listener;
	button->listener = this->listener;

	background->function = this->function;
	button->function = this->function;
	return true;
}

//Called every frame after the element update
bool j1UISlider::InheritUpdate(float dt)
{
	//The childs follow the father's enabled
	button->enabled = this->enabled;
	background, enabled = this->enabled;

	//Check Bounadries
	CheckLimits();

	return true;
}

// Called before all Updates
bool j1UISlider::PostUpdate(bool debug)
{
	Draw(debug);
	return true;
}

// Called before quitting
bool j1UISlider::CleanUp()
{	
	texture = nullptr;

	button = nullptr;
	background = nullptr;

	return true;
}

//Check if the slider button goes pass the limits & reposition it
void j1UISlider::CheckLimits()
{
	// Left
	if (button->PostoParent.x > 0)
	{
		button->PostoParent.x = 0;
		UpdatePosition();

	}
	//Right
	else if (button->PostoParent.x < (-background->rect.w + button->rect.w))
	{
		button->PostoParent.x = -background->rect.w + button->rect.w;
		UpdatePosition();
	}
}

//Get the value (& translate it to audio) from the button
int j1UISlider::GetAudioValue()
{
	return round((-(float(-button->PostoParent.x) / ((float(-background->rect.w)) + float(button->rect.w))) * 128));
}
