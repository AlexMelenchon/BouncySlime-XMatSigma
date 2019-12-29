#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Window.h"
#include "SDL/include/SDL.h"
#include "j1Fonts.h"
#include "j1Scene.h"
#include "j1UIelement.h"
#include "j1ConsoleM.h"

#define MAX_KEYS 300

//Constructor
j1Input::j1Input() : j1Module()
{
	name.create("input");

	keyboard = new j1KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(j1KeyState) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(j1KeyState) * NUM_MOUSE_BUTTONS);
}

// Destructor
j1Input::~j1Input()
{
	delete[] keyboard;
}

// Called before render is available
bool j1Input::Awake(pugi::xml_node& config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	App->console->CreateCommand("quit", this, 1, 1, UIFunction::FNC_QUIT);

	return ret;
}

// Called before the first frame
bool j1Input::Start()
{
	SDL_StopTextInput();
	return true;
}

// Called each loop iteration / all keys states' control
bool j1Input::PreUpdate()
{
	BROFILER_CATEGORY("Input Pre-Update", Profiler::Color::Chartreuse)

	static SDL_Event event;

	if (windowEvents[WE_QUIT] == true)
		return false;
	

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	//If we are not writting, we disable the "gameplay" inputs & we just focus on the writting events
	if (!writting)
	{
		for (int i = 0; i < MAX_KEYS; ++i)
		{
			if (keys[i] == 1)
			{
				if (keyboard[i] == KEY_IDLE)
					keyboard[i] = KEY_DOWN;
				else
					keyboard[i] = KEY_REPEAT;
			}
			else
			{
				if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
					keyboard[i] = KEY_UP;
				else
					keyboard[i] = KEY_IDLE;
			}
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if (mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}

	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			Quit(true);
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
				//case SDL_WINDOWEVENT_LEAVE:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[WE_HIDE] = true;
				break;

				//case SDL_WINDOWEVENT_ENTER:
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[WE_SHOW] = true;
				break;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouse_buttons[event.button.button - 1] = KEY_DOWN;
			//LOG("Mouse button %d down", event.button.button-1);
			break;

		case SDL_MOUSEBUTTONUP:
			mouse_buttons[event.button.button - 1] = KEY_UP;
			//LOG("Mouse button %d up", event.button.button-1);
			break;

		case SDL_TEXTINPUT:

			//If the square text is full, we don't write anyting & we notice the puser
			if (GetTextWidth() >= inputRect_w)
			{
				LOG("Max console capacity exceeded!");
			}
			else
			{
				//We write in the cursor position
				if (cursorPosition == 0)
					textString += event.text.text;
				else
				{
					textString.insert(event.text.text, cursorPosition);
				}

			}
			break;

		case SDL_KEYDOWN:
			//If we are not writting, we are going to ignore this, since we don't use them for gameplay
			if (writting)
			{
				//Deletes the left letter of the cursor when the backspace is pressed
				if (event.key.keysym.sym == SDLK_BACKSPACE && textString.Length() > 0)
				{
					textString.Cut(textString.Length() - (cursorPosition)-1, textString.Length() - cursorPosition);
				}

				//Deletes the right letter of the cursor when supr is pressed
				else if (event.key.keysym.sym == SDLK_DELETE && textString.Length() > 0)
				{
					textString.Cut(textString.Length() - (cursorPosition), textString.Length() - cursorPosition + 1);
					if (cursorPosition > 0)
						cursorPosition--;
				}

				//Moves though the written string to the left
				if (event.key.keysym.sym == SDLK_LEFT && cursorPosition < textString.Length())
				{
					cursorPosition++;
				}

				//Moves though the written string to the right
				else if (event.key.keysym.sym == SDLK_RIGHT && cursorPosition > 0)
				{
					cursorPosition--;
				}

				//Closes the console
				if (event.key.keysym.sym == SDLK_BACKQUOTE)
				{
					App->input->WrittingState(false);
					App->scene->console->SetToDisable(!App->scene->console->enabled);
				}

				//Enters a command into the console
				if (event.key.keysym.sym == SDLK_RETURN)
				{
					App->scene->console->RecieveCommand(textString.GetString());
					cursorPosition = 0;
					textString.Clear();
				}

			}
			break;
		case SDL_MOUSEMOTION:
			int scale = App->win->GetScale();
			mouse_motion_x = event.motion.xrel / scale;
			mouse_motion_y = event.motion.yrel / scale;
			mouse_x = event.motion.x / scale;
			mouse_y = event.motion.y / scale;
			break;
		}
	}

	return true;
}

// Called before quitting
bool j1Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	return true;
}

// ---------
bool j1Input::GetWindowEvent(j1EventWindow ev)
{
	return windowEvents[ev];
}

void j1Input::GetMousePosition(int& x, int& y)
{
	x = mouse_x;
	y = mouse_y;
}

void j1Input::GetMouseMotion(int& x, int& y)
{
	x = mouse_motion_x;
	y = mouse_motion_y;
}

//Sets all keys to not being pressed
void j1Input::ReSetKeys()
{
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		keyboard[i] = KEY_UP;
	}
}

//Returns the buffer string
const char* j1Input::GetText()
{
	return textString.GetString();
}

//Returns the size of the buffer string in a certain position
int j1Input::GetTextInPos()
{
	int tmpW, tmpH = 0;

	App->fonts->CalcSize(textString.GetStringInPos(cursorPosition), tmpW, tmpH);

	if (tmpW < -TMP_STRING_SIZE) tmpW = 0;

	return tmpW;

}

//Returns the size of the text font, for multi-line purposes
int j1Input::GetTextWidth()
{
	int tmpW = 1;
	int tmpH = 1;

	if(textString.Length() > 2)
	App->fonts->CalcSize(textString.GetString(), tmpW, tmpH);

	return tmpW;
}

//Changes the input mode from wirtting to playing
void j1Input::WrittingState(bool state, SDL_Rect rect)
{
	if (writting != state)
	{
		writting = !writting;

		if (writting)
		{
			SDL_StartTextInput();
			SDL_SetTextInputRect(&rect);
			inputRect_w = rect.w * 0.99f;
			ReSetKeys();
			TTF_GlyphMetrics(App->fonts->default_font, 1, 0, &rect.w, 0, &rect.w, 0);
		}
		else
		{
			SDL_StopTextInput();
			cursorPosition = 0;
			textString.Clear();
			inputRect_w = 0;
		}
	}
}

//Custom Disable (cleanup the module & it's updates are not done till is activated again)
void j1Input::Disable()
{
	if (active == true)
	{
		active = false;
		ReSetKeys();
	}
}

//Custom enable (calls the module Start() & make it active again)
void j1Input::Enable()
{
	if (active == false)
	{
		active = true;
		ReSetKeys();
	}

}

//used to say to the game we want quit
void j1Input::Quit(bool scheduleToQuit)
{
	if (scheduleToQuit)
		windowEvents[WE_QUIT] = true;
}


//Manages the UI inputs of this module
void j1Input::OnGui(UIEventType type, UIFunction func, j1UIelement* userPointer, const char* bufferText)
{
	switch (type)
	{
	case UIEventType::EVENT_CONSOLE:

		switch (func)
		{
		case UIFunction::FNC_QUIT:
			Quit(true);
			LOG("Quitting...");
			LOG("Thank you for playing Bouncy Slime! Please check out our GitHub Project for more Info :)");
			break;
		}
	}
}