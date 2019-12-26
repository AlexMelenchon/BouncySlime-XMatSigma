#ifndef __J1__UISLIDER__
#define __J1__UISLIDER__

#include"j1UIelement.h"
#include "p2Point.h"

class j1UISlider : public j1UIelement
{
public:
	j1UISlider();
	j1UISlider(drag_axis axis);
	~j1UISlider();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool InheritUpdate(float dt);
	bool PostUpdate(bool debug);

	// Called before quitting
	bool CleanUp();
	
	//--------SAVE & LOAD---------//
	//Called when loading a save
	bool Load(pugi::xml_node&) { return true; };

	//Called to save the game
	bool Save(pugi::xml_node&) const  const { return true; };

	void CheckLimits();

	int GetAudioValue();


private:
	j1UIelement* thumb = nullptr;
	j1UIelement* line = nullptr;
};


#endif // !__J1__UIINPUTBOX__