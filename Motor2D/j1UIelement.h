#ifndef __J1UIELEMENT__
#define __J1UIELEMENT__

#include "j1Entity.h"
#include "p2Defs.h"
#include "p2List.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "SDL/include/SDL_mouse.h"

//Enum for element types
enum class ui_type
{
	UI_NONE,
	UI_BUTTON,
	UI_IMAGE,
	UI_TEXT,
	UI_INPUTBOX,
	UI_SLIDER
};


enum class drag_axis
{
	MOV_NONE,
	MOV_ALL,
	MOV_X,
	MOV_Y
};

class j1UIelement
{
public:
	//--------INTERNAL CONTROL---------//

	//Constructor
	j1UIelement();

	//Destructors
	~j1UIelement();

	// Called before the first frame
	virtual bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);
	//Custom update fuction for inheritances that might need it
	virtual bool InheritUpdate(float dt) { return true; }

	// Called before all Updates
	virtual bool PostUpdate(bool debug) { return true; };

	// Called before quitting
	virtual bool CleanUp();

	//-----------DRAW-------------//
	//Draws the element into the screen
	virtual bool Draw(bool debug);


	//-----ELEMENT MANAGEMENT-----//
	//Checks if the mouse if hovering the element & if it should hover over it's siblings or parents
	bool OnHover();

	//Handles inputs in when the element is pressed, released & dragged respectively--------
	virtual void OnClick();
	virtual bool OnRelease();
	virtual void OnDrag();

	//Move the element
	virtual void Move(float dt);

	//Make sure that, if the element has a parent, they maintain their distance to it
	void KeepDistanceToParent(float dt);

	//Gets the value from certain positions
	virtual int GetAudioValue() { return 0; }

	//Checks if the element has the focus or not
	bool IsFocused();

	//Retrives focus when the conditions are met
	virtual bool DeFocus();

	//Used to Update the elements position when it's not moving
	void UpdatePosition();

public:
	//--------UI LOGIC-------------//

	//If the object is intractuable (can be clicked & calls for functions)
	bool interact = false;

	//Flag if the object is draggeable
	bool drag = false;

	//Flag if the object is enabled (if not, the draw & logic is de-activated)
	bool enabled = false;

	//Flags to know if the element in being hovered or dragged
	bool hovering = false;
	bool dragging = false;

	//Flag to set-up volatile menus
	bool to_delete = false;

	//--------TEXTURE-------------------//

	//Section of the texture, as well as size
	SDL_Rect rect = { 0,0,0,0 };
	
	//Elements texture (it will always point to atlas :D)
	SDL_Texture* texture = nullptr;

	//-----------DISTANCE---------------//
	//Relative position to the parent
	iPoint PostoParent = { 0,0 };

	//Global Position on Screen
	iPoint Position = { 0,0 };

	//The point in which the object has been clicked
	iPoint MovePoint = { 0,0 };

	//--------UI MANAGEMENT-------------//

	//Pointer to the element's parent, if any
	j1UIelement* parent = nullptr;

	//The ui type
	ui_type type = ui_type::UI_NONE;

	//The axis in which the object can be dragged
	drag_axis axis = drag_axis::MOV_NONE;

	//The module that the element will call on a fuction case
	j1Module* listener = nullptr;

	//The fuction that the element does
	UIFunction function = UIFunction::FNC_NONE;

	//Text, if the element needs it
	char* text = nullptr;

};

#endif // !__J1UIELEMENT__

