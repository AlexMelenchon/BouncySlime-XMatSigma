#ifndef __J1__UITEXT__
#define __J1__UITEXT__

#include"j1UIelement.h"

class j1UIText : public j1UIelement
{
public:
	j1UIText();
	j1UIText(char* t);

	~j1UIText();

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

};


#endif // !__J1__UITEXT__
