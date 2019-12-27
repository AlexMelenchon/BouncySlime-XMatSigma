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

	void NoTextDraw(bool debug);

};


#endif // !__J1__UITEXT__
