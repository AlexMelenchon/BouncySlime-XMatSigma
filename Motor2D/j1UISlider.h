#ifndef __J1__UISLIDER__
#define __J1__UISLIDER__

#include"j1UIelement.h"
#include "p2Point.h"

class j1UISlider : public j1UIelement
{
public:
	//--------INTERNAL CONTROL---------//

	//Constructors-----
	j1UISlider();
	j1UISlider(drag_axis axis);

	//Destructors
	~j1UISlider();

	// Called before the first frame when created
	bool Start();

	//Called every frame after the element update
	bool InheritUpdate(float dt);

	// Called before all Updates
	bool PostUpdate(bool debug);

	// Called before quitting
	bool CleanUp();
	
	//--------SLIDER MANAGEMENT---------//

	//Check if the slider button goes pass the limits & reposition it
	void CheckLimits();

	//Get the value (& translate it to audio) from the button
	int GetAudioValue();


private:
	//--------SLIDER CHILDS---------//

	j1UIelement* thumb = nullptr;
	j1UIelement* line = nullptr;
};


#endif // !__J1__UIINPUTBOX__