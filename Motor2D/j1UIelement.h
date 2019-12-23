#ifndef __J1UIELEMENT__
#define __J1UIELEMENT__

#include "j1Entity.h"
#include "p2Defs.h"
#include "p2List.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "SDL/include/SDL_mouse.h"


enum class ui_type
{
	UI_NONE,
	UI_BUTTON,
	UI_IMAGE,
	UI_TEXT,
	UI_INPUTBOX
};

class j1UIelement
{
public:

	j1UIelement();

	~j1UIelement();

	virtual bool Awake(pugi::xml_node&) { return true; };;

	// Called before the first frame
	virtual bool Start();

	// Called each loop iteration
	virtual bool PreUpdate() { return true; };

	// Called each loop iteration
	virtual bool Update(float dt);

	virtual bool PostUpdate(bool debug) { return true; };

	// Called before quitting
	virtual bool CleanUp() { return true; };


	//--------DRAW--------//
	virtual void Draw(bool debug);

	//--------SAVE & LOAD---------//
	//Called when loading a save
	virtual bool Load(pugi::xml_node&) { return true; };

	//Called to save the game
	virtual bool Save(pugi::xml_node&) const  const { return true; };

	//-----EXTERNAL-----//
	bool OnHover();
	virtual void OnClick();
	virtual void OnRelease();
	virtual void OnDrag();
	virtual void Move(float dt);
	virtual void KeepDistanceToParent(float dt);
	

public:

	bool interact = false;
	bool drag = false;
	bool enabled = false;

	SDL_Rect rect = { 0,0,0,0 };
	SDL_Texture* texture = nullptr;

	iPoint PostoParent = { 0,0 };
	iPoint Position = { 0,0 };
	iPoint MovePoint = { 0,0 };

	j1UIelement* parent = nullptr;
	ui_type type = ui_type::UI_NONE;
	

	j1Module* listener = nullptr;
	UIFunction function = UIFunction::FNC_UNKOWNN;

	bool hovering = false;
	bool dragging = false;


};

#endif // !__J1UIELEMENT__

