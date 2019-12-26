#ifndef __J1__UIBUTTON__
#define __J1__UIBUTTON__

#include"j1UIelement.h"



class j1UIButton : public j1UIelement
{
public:
	j1UIButton();

	j1UIButton(const char* text);

	
	~j1UIButton();

	bool Awake(pugi::xml_node&);

	// Called each loop iteration
	//bool PreUpdate();

	// Called each loop iteration
	bool InheritUpdate(float dt);

	bool PostUpdate(bool debug);

	void OnDrag();


	//--------SAVE & LOAD---------//
	//Called when loading a save
	bool Load(pugi::xml_node&) { return true; };

	//Called to save the game
	bool Save(pugi::xml_node&) const  const { return true; };

};


#endif // !__J1__UIBUTTON__
