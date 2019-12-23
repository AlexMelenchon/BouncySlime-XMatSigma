#ifndef __J1__UIINPUTBOX__
#define __J1__UIINPUTBOX__

#include"j1UIelement.h"
#include "p2Point.h"

class j1UIInputBox : public j1UIelement
{
public:
	j1UIInputBox();
	j1UIInputBox(const char* txt);

	~j1UIInputBox();

	bool Awake(pugi::xml_node&);

	bool Start();


	bool PostUpdate(bool debug);

	// Called before quitting
	bool CleanUp();


	//--------SAVE & LOAD---------//
	//Called when loading a save
	bool Load(pugi::xml_node&) { return true; };

	//Called to save the game
	bool Save(pugi::xml_node&) const  const { return true; };


private:

	int w, h;

};


#endif // !__J1__UIINPUTBOX__