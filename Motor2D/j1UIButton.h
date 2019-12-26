#ifndef __J1__UIBUTTON__
#define __J1__UIBUTTON__

#include"j1UIelement.h"



class j1UIButton : public j1UIelement
{
public:
	//--------INTERNAL CONTROL---------//

	//Constructors----
	j1UIButton();
	j1UIButton(char* text);

	//Destructors
	~j1UIButton();

	// Called before the first frame when created
	bool Start();

	// Called before all Updates
	bool PostUpdate(bool debug);

	//Custom OnDrag calling function
	void OnDrag();

};


#endif // !__J1__UIBUTTON__
