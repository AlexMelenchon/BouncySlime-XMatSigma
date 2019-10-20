#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "p2Point.h"

#define MAX_FRAMES 40

class Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
	float speedinframes = 1.0f;
	int framesXframe[MAX_FRAMES]; //array of time elapsed in frames for each step of the animation

	float firstLoopFrame = 0.0f;
	SDL_Rect frames[MAX_FRAMES];


public:
	float current_frame;
	int last_frame = 0;
	bool animEnded = false;
	int loops = 0;

public:

	void PushBack(const SDL_Rect& rect, int frXfr)
	{
		framesXframe[last_frame] = frXfr;
		frames[last_frame++] = rect;
	}

	void PushBack(pugi::xml_node framesIterator)
	{

		for (framesIterator = framesIterator.child("sprite"); framesIterator; framesIterator = framesIterator.next_sibling("sprite"))
		{
			SDL_Rect frame;
			frame.x = framesIterator.attribute("x").as_int();
			frame.y = framesIterator.attribute("y").as_int();
			frame.w = framesIterator.attribute("w").as_int();
			frame.h = framesIterator.attribute("h").as_int();

			framesXframe[last_frame] = framesIterator.attribute("frames").as_int();
			frames[last_frame++] = frame;
		}

	}

	SDL_Rect& GetCurrentFrame()
	{//returns the rectangle displaying the current animation
		//current_frame += speed;
		float auxframe2 = current_frame;
		float auxframe = (1.0f / (framesXframe[(int)current_frame]));
		current_frame += auxframe;// frames per frame (24 frames x sec) 
		//!!have to do it for the actual game speed
		if (current_frame >= last_frame) {
			current_frame = (loop) ? firstLoopFrame : -1;
			animEnded = true;
			loops++;
		}
		return frames[(int)auxframe2];
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