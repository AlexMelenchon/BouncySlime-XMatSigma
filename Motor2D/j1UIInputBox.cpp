#include "j1UIInputBox.h"
#include "j1UIManager.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Textures.h"

//Constructors------
j1UIInputBox::j1UIInputBox()
{}

j1UIInputBox::j1UIInputBox(char* txt)
{
	this->type = ui_type::UI_INPUTBOX;

	//Load the childs----
	boxImage = App->ui->AddElement(ui_type::UI_IMAGE, this, this->Position, true, false, true, { 73, 992, 256, 64 }, this->listener, UIFunction::FNC_NONE);
	boxText = App->ui->AddElement(ui_type::UI_TEXT, this, this->Position, true, false, true, this->rect, this->listener, UIFunction::FNC_NONE, this->axis, txt);
}

//Destructor
j1UIInputBox::~j1UIInputBox()
{}


//Called every frame after the element update
bool j1UIInputBox::InheritUpdate(float dt)
{
	cursorTimer += dt;

	if (cursorTimer > 1.5f)
		cursorTimer = 0;

	return true;
}

// Called before all Updates
bool j1UIInputBox::PostUpdate(bool debug)
{
	//If it's focused....
	if (this->IsFocused())
	{
		//We say to the input that we're writting
		App->input->WrittingState(true, boxImage->rect);

		//We draw the cursor in it's position w/ a blink
		int cursorPosition = App->input->GetTextInPos();
		if (cursorTimer > 0.75f)
		{
			App->render->DrawQuad({ boxText->Position.x + cursorPosition, boxText->Position.y,4, boxText->rect.h }, 200, 200, 200, 255, true, false);
		}
	}
	else
		App->input->WrittingState(false, boxImage->rect);

	return true;
}

// Called before quitting
bool j1UIInputBox::CleanUp()
{	
	//Reset the pointers
	texture = nullptr;
	boxImage = nullptr;
	boxText = nullptr;
	return true;
}

//Retrives focus when the conditions are met
void j1UIInputBox::DeFocus()
{
	//When the player clicks outside the inputBox, we stop focusing on it
	// TODO: when the input box closes, also do this
	if(App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && !this->hovering)
		App->ui->focused.lookAt = nullptr;
}
