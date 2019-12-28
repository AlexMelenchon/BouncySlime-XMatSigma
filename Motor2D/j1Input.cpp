#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Window.h"
#include "SDL/include/SDL.h"
#include "j1Fonts.h"
#include "j1Scene.h"
#include "j1UIelement.h"

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

	const Uint8* keys = SDL_GetKeyboardState(NULL);

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
			windowEvents[WE_QUIT] = true;
			return false;
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
			if ((GetTextSize().x) > inputRect.w)
			{
				LOG("Max console capacity exceeded!");
			}
			else
			{
				if (position == 0)
					textString += event.text.text;
				else
				{
					textString.insert(event.text.text, position);
				}

			}
			break;

		case SDL_KEYDOWN:
			if (writting)
			{

				SDL_Keycode hpña = event.key.keysym.sym;

				if (event.key.keysym.sym == SDLK_BACKSPACE && textString.Length() > 0)
				{
					textString.Cut(textString.Length() - (position)-1, textString.Length() - position);
				}

				else if (event.key.keysym.sym == SDLK_DELETE && textString.Length() > 0)
				{
					textString.Cut(textString.Length() - (position), textString.Length() - position + 1);
					if (position > 0)
						position--;
				}

				if (event.key.keysym.sym == SDLK_LEFT && position < textString.Length())
				{
					position++;
				}

				else if (event.key.keysym.sym == SDLK_RIGHT && position > 0)
				{
					position--;
				}

				if (event.key.keysym.sym == SDLK_BACKQUOTE)
				{
					App->input->WrittingState(false, this->inputRect);
					App->scene->console->Disable(!App->scene->console->enabled);
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

const char* j1Input::GetText()
{
	return textString.GetString();
}

int j1Input::GetTextInPos()
{
	int tmpW, tmpH = 0;

	App->fonts->CalcSize(textString.GetStringInPos(position), tmpW, tmpH);

	if (tmpW < -TMP_STRING_SIZE) tmpW = 0;

	return tmpW;

}

iPoint j1Input::GetTextSize()
{
	int tmpW, tmpH = 0;

	App->fonts->CalcSize(textString.GetString(), tmpW, tmpH);

	return { tmpW , tmpH };

}

void j1Input::WrittingState(bool state, SDL_Rect rect)
{
	if (writting != state)
	{
		writting = !writting;

		if (writting)
		{
			SDL_StartTextInput();
			SDL_SetTextInputRect(&rect);
			inputRect = { rect.x, rect.y, rect.w, rect.h };
			ReSetKeys();
			TTF_GlyphMetrics(App->fonts->default_font, 1, 0, &rect.w, 0, &rect.w, 0);
		}
		else
		{
			SDL_StopTextInput();
			position = 0;
			textString.Clear();
			inputRect = { 0,0,0,0 };
		}
	}
}

void j1Input::Disable()
{
	if (active == true)
	{
		active = false;
		ReSetKeys();
	}
}

void j1Input::Enable()
{
	if (active == false)
	{
		active = true;
		ReSetKeys();
	}

}