#include "j1UIImage.h"

//Constructor
j1UIImage::j1UIImage()
{
	this->type = ui_type::UI_IMAGE;
}

//Destructor
j1UIImage::~j1UIImage()
{}

// Called before all Updates
bool j1UIImage::PostUpdate(bool debug)
{
	Draw(debug);
	return true;
}
