#ifndef __J1__UITEXT__
#define __J1__UITEXT__

#include"j1UIelement.h"

class j1UIText : public j1UIelement
{
public:
	j1UIText();
	j1UIText(const char* t);

	~j1UIText();

	bool Awake(pugi::xml_node&);

	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	bool PostUpdate(bool debug);

	// Called before quitting
	bool CleanUp();

	void Draw(bool debug);

	//--------SAVE & LOAD---------//
	//Called when loading a save
	bool Load(pugi::xml_node&) { return true; };

	//Called to save the game
	bool Save(pugi::xml_node&) const  const { return true; };


private:

	int w, h;

};


#endif // !__J1__UITEXT__
