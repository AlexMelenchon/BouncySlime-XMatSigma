#ifndef __UIMANAGER__
#define __UIMANAGER__


#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "p2Defs.h"
#include "j1UIelement.h"

class j1UIelement;


class j1UIManager : public j1UIelement
{

	j1UIManager();
	~j1UIManager() {}


// Called before render is available
bool Awake(pugi::xml_node&);

// Called before the first frame
bool Start();

// Called each loop iteration
bool PreUpdate();

// Called each loop iteration
bool Update(float dt);

// Called each loop iteration
bool PostUpdate();

// Called before quitting
bool CleanUp();


//--------SAVE & LOAD---------//


j1UIelement* AddElement(ui_type type, j1UIelement* parent, iPoint gloalPos = { 0,0 }, iPoint localPos = { 0,0 }, bool interact = false, bool drag = false, bool enabled = false);
void DeleteElement(j1UIelement* element);

j1UIelement* IsOnElement(int x, int y);


private:
	//--------ENTITIES---------//
	//The list where we will store all the entities
	p2List<j1UIelement*> UIList;

};



#endif // !__UIMANAGER__

