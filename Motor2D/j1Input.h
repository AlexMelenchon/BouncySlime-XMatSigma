#ifndef __j1INPUT_H__
#define __j1INPUT_H__

#include "j1Module.h"

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
//#define LAST_KEYS_PRESSED_BUFFER 50

struct SDL_Rect;

enum j1EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum j1KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

enum class InputState
{
	ST_UNKNOWN = -1,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class j1Input : public j1Module
{

public:
	//--------INTERNAL CONTROL---------//

	//Constructor
	j1Input();

	// Destructor
	virtual ~j1Input();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	//Custom enable & disable
	void Disable();
	void Enable();

	//--------KEY CONTROL---------//
	// Gather relevant win events
	bool GetWindowEvent(j1EventWindow ev);

	// Check key states (includes mouse and joy buttons)
	j1KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	j1KeyState GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}

	//used to say to the game we want quit
	void Quit(bool scheduleToQuit);

	// Check if a certain window event happened
	bool GetWindowEvent(int code);

	//Sets all keys to KEY_UP
	void ReSetKeys();

	// Get mouse / axis position
	void GetMousePosition(int& x, int& y);
	//Get mouse motion
	void GetMouseMotion(int& x, int& y);

	//Returns the buffer string
	const char* GetText();

	//Returns the size of the buffer string in a certain position
	int GetTextInPos();

	//Returns the size of the text font, for multi-line purposes
	int GetTextWidth();

	//Changes the input mode from wirtting to playing
	void WrittingState(bool state, SDL_Rect rect = {0,0,0,0});

	// Get mouse / axis position
	int			mouse_x;
	int			mouse_y;

	//--------UI----------------------//
	//Manages the UI inputs of this module
	void OnGui(UIEventType type, UIFunction func, j1UIelement* userPointer = nullptr, const char* bufferText = "");

private:

	//--------EVENT CONTROL---------//
	bool		windowEvents[WE_COUNT];

	//--------KEY CONTROL---------//
	j1KeyState* keyboard;
	j1KeyState	mouse_buttons[NUM_MOUSE_BUTTONS];

	// Get mouse / axis motion  position
	int			mouse_motion_x;
	int			mouse_motion_y;

	bool		writting = false;

	int inputRect_w =  0;


	//--------BUFFER STRING---------//
	p2SString		textString;
	int cursorPosition = 0;
};

#endif // __j1INPUT_H__