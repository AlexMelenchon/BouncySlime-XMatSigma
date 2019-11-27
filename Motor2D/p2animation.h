#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "p2Point.h"
#include "PugiXml\src\pugixml.hpp"
#include "p2SString.h"

#define MAX_FRAMES 50

class Animation
{
public:
	bool loop = true; // Set to flase in pushback so the animation does not loop
	float speed = 1.0f; //Animation speed;
	int framesXframe[MAX_FRAMES]; //array of time elapsed in frames for each step of the animation
	iPoint pivotpos[MAX_FRAMES]; //array of (x,y) coordinates, containing the pivot for each animation frame
	float firstLoopFrame = 0.0f; //The frame to loop

	SDL_Rect frames[MAX_FRAMES]; //Number of frames per animation


public:
	float current_frame;
	int last_frame = 0;
	bool animEnded = false;
	int loops = 0;

public:


  //Loads a frae of an animation
	void PushBack(const SDL_Rect& rect, int frXfr, int pivotX, int pivotY)
	{
		framesXframe[last_frame] = frXfr;
		pivotpos[last_frame].x = pivotX; 
		pivotpos[last_frame].y = pivotY;	
		frames[last_frame] = rect;
		last_frame++;
	}

	//Searchs if an animation exists, then loads it
	void loadAnimation(pugi::xml_node animationIterator,p2SString name)
	{
		for (animationIterator = animationIterator; animationIterator != NULL; animationIterator = animationIterator.next_sibling("animation"))
		{
			if (name == animationIterator.attribute("name").as_string())
			{
				for (pugi::xml_node framesIterator = animationIterator.child("sprite"); framesIterator; framesIterator = framesIterator.next_sibling("sprite"))
				{
					SDL_Rect frame;
					frame.x = framesIterator.attribute("x").as_int();
					frame.y = framesIterator.attribute("y").as_int();
					frame.w = framesIterator.attribute("w").as_int();
					frame.h = framesIterator.attribute("h").as_int();

					this->PushBack(frame, framesIterator.attribute("frames").as_int(), framesIterator.attribute("pivotx").as_int(), framesIterator.attribute("pivoty").as_int() );
				}
				break;
			}
		}
	}

	SDL_Rect& GetCurrentFrame(float dt)
	{
		//returns the rectangle displaying the current animation
		//TODO animation acording to speed
		current_frame += speed * dt * 3;
		//!!have to do it for the actual game speed
		if (current_frame >= last_frame) {
			current_frame = (loop) ? firstLoopFrame : -1;
			animEnded = true;
			loops++;
		}
		return frames[(int)current_frame];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		current_frame = 0;
	}

	bool GetAnimEnd() { return animEnded; }
	void SetAnimEnd(bool end) { animEnded = end; }
};

#endif
