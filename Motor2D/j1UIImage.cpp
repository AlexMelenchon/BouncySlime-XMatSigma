#include "j1UIImage.h"

j1UIImage::j1UIImage()
{
	this->type = ui_type::UI_IMAGE;
}

j1UIImage::~j1UIImage()
{}

bool j1UIImage::Awake(pugi::xml_node&)
{
	return true;
}

bool j1UIImage::Start()
{
	return true;
}

bool j1UIImage::PreUpdate()
{
	return true;
}

bool j1UIImage::Update(float dt)
{
	return true;
}

bool j1UIImage::PostUpdate(bool debug)
{
	Draw(debug);
	return true;
}

bool j1UIImage::CleanUp()
{
	return true;
}
