#ifndef __J1__CONSOLE__
#define __J1__CONSOLE__

#include"j1UIelement.h"
#include "p2Point.h"

class j1Console : public j1UIelement
{
public:
	//--------INTERNAL CONTROL---------//
	//Constructors----
	j1Console();
	j1Console(char* txt, iPoint Position);

	//Destructors
	~j1Console();

	//Called every frame after the element update
	bool InheritUpdate(float dt);

	// Called before all Updates
	bool PostUpdate(bool debug);

	// Called before quitting
	bool CleanUp();

	//--------INTERNAL MANAGEMENT---------//
	//Retrives focus when the conditions are met
	bool DeFocus();

private:
	//--------INTERNAL MANAGEMENT---------//
	//Input box childs------
	j1UIelement* inputBox = nullptr;
	j1UIelement* image = nullptr;
};


#endif // !__J1__CONSOLE__