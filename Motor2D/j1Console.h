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

	//Called when the console is disabled
	void Disable();

	//----------CONSOLE RELATED-------------//
	//Updates the text to show
	void UpdateText(const char* newLogEntry);

	//Gets the text from the input
	void RecieveCommand(const char* newCommand);

private:
	//--------INTERNAL MANAGEMENT---------//
	//Input box childs------
	j1UIelement* inputBox = nullptr;
	j1UIelement* image = nullptr;
	j1UIelement* Ctext = nullptr;

	p2SString consoleBuffer;
};


#endif // !__J1__CONSOLE__