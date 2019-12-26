#ifndef __J1__UIIMAGE__
#define __J1__UIIMAGE__

#include"j1UIelement.h"

class j1UIImage : public j1UIelement
{
public:
	j1UIImage();

	~j1UIImage();

	bool Awake(pugi::xml_node&);	

	bool PostUpdate(bool debug);


	//--------SAVE & LOAD---------//
	//Called when loading a save
	bool Load(pugi::xml_node&) { return true; };

	//Called to save the game
	bool Save(pugi::xml_node&) const  const { return true; };


private:

	int w, h;

};


#endif // !__J1__UITEXT__
