#ifndef __J1__UIINPUTBOX__
#define __J1__UIINPUTBOX__

#include"j1UIelement.h"
#include "p2Point.h"

class j1UIInputBox : public j1UIelement
{
public:
	//--------INTERNAL CONTROL---------//
	//Constructors----
	j1UIInputBox();
	j1UIInputBox(char* txt);

	//Destructors
	~j1UIInputBox();

	//Called every frame after the element update
	bool InheritUpdate(float dt);

	// Called before all Updates
	bool PostUpdate(bool debug);

	// Called before quitting
	bool CleanUp();

	//--------INTERNAL MANAGEMENT---------//
	//Retrives focus when the conditions are met
	void DeFocus();

private:
	//--------INTERNAL MANAGEMENT---------//
	//Input box childs------
	j1UIelement* boxImage = nullptr;
	j1UIelement* boxText = nullptr;

	//The time elapsed that the cursor blinks
	float cursorTimer = 0;
};


#endif // !__J1__UIINPUTBOX__