#ifndef __J1__UIIMAGE__
#define __J1__UIIMAGE__

#include"j1UIelement.h"


class j1UIImage : public j1UIelement
{
public:
	//--------INTERNAL CONTROL---------//
	//Constructor
	j1UIImage();

	//Destructor
	~j1UIImage();

	// Called before the first frame when created
	bool Start();

	// Called before all Updates
	bool PostUpdate(bool debug);

	// Used to Draw when the image has no texture
	void NoTextDraw(bool debug);

	bool DeFocus();

private:
	//Color for the image when it has no texture
	SDL_Color noTextColor = {255,255,255,255};

};


#endif // !__J1__UITEXT__
