#ifndef __UIMANAGER__
#define __UIMANAGER__


#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "p2Defs.h"
#include "j1UIelement.h"
#include "j1Command.h"

class j1UIelement;

//Enum for the focus logic
enum class focusState
{
	ST_UNKNOWN  = -1,

	ST_LOCKED,
	ST_FREE

};

//Struct to handle the focused element
struct focus
{
	p2List_item<j1UIelement*>* lookAt = nullptr;
	focusState state = focusState::ST_FREE;
};

class j1UIManager : public j1Module
{
public:
	//------------Methods--------------------//

	//--------INTERNAL CONTROL---------//

	//Constructor
	j1UIManager();

	//Destructor
	~j1UIManager();

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

	//--------UI MANAGEMENT---------//

	//Adds an UI element into the list
	j1UIelement* AddElement(ui_type type,
		j1UIelement* parent = nullptr,
		iPoint Position = { 0,0 },
		bool interact = false,
		bool drag = false,
		bool enabled = false,
		SDL_Rect section = { 0,0,0,0 },
		j1Module* listener = nullptr,
		UIFunction func = UIFunction::FNC_NONE,
		drag_axis axis = drag_axis::MOV_ALL,
		char* text = ""
	);

	//Initializes UI elements
	void InitElement(j1UIelement* element, pugi::xml_node);

	//Deletes an specific element
	void DeleteElement(p2List_item<j1UIelement*>* element);

	//Deletes the elements with the "to_delete" flag
	void ToDeleteElement();

	//Deletes all UI elements
	void DeleteAllElements();

	//Gets an element from the UI list
	p2List_item<j1UIelement*>* GetElementFromList(j1UIelement* toSearch);

	//Changes the focus through elements pressing TAB
	void ChangeFocus();

	//Get the atlas texture
	SDL_Texture* GetAtlas() const;

	//-----------INNER LOGIC-----------//
	bool debug = false;
	focus focused;

private:
	//--------ELEMENTS---------//
	//The list where we will store all the entities
	p2List<j1UIelement*> UIList;

	//--------ATLAS---------//
	SDL_Texture* atlas;
	p2SString atlas_file_name;

};


#endif // !__UIMANAGER__

