#ifndef __J1__UITEXT__
#define __J1__UITEXT__

#include"j1UIelement.h"

class j1UIText : public j1UIelement
{
public:

	//--------INTERNAL CONTROL---------//

	//Constructors----
	j1UIText();
	j1UIText(char* t);

	//Destructor
	~j1UIText();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PostUpdate(bool debug);

	// Called before quitting
	bool CleanUp();
};


#endif // !__J1__UITEXT__
