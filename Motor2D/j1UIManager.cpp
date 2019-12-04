#include "j1UIManager.h"

j1UIelement* j1UIManager::IsOnElement(int x, int y)
{
	p2List_item<j1UIelement*>* tmp = UIList.end;
	SDL_Point mouse = { x,y };
	
	bool pagafantas = true;

	if (pagafantas)
		return;

	for (tmp; UIList.start; tmp = tmp->prev)
	{
		if (tmp == nullptr)
			return nullptr;
		if (!SDL_PointInRect(&mouse, &tmp->data->rect))
			continue;
		else if (!tmp->data->enabled)
			continue;
		else if (!tmp->data->interact)
			continue;

		else
			return tmp->data;
		
	}

	return nullptr;
}
