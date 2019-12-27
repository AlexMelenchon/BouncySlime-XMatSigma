#include "j1Coin.h"
#include "j1Collision.h"



// Called before render is available
bool j1Coin::Awake(pugi::xml_node& coin_node)
{
	//Create the player's collider
	scalesize = coin_node.child("collision").child("scalesize").text().as_uint();

	SDL_Rect particleRect = { 0,0,0,0 };
	particleRect.w = coin_node.child("collision").child("collider").attribute("w").as_float() * scalesize;
	particleRect.h = coin_node.child("collision").child("collider").attribute("h").as_float() * scalesize;

	//collider load
	collider = new Collider(particleRect, COLLIDER_SHURIKEN, this);

	//animation load
	pugi::xml_node animIterator = coin_node.child("animations").child("animation");
	anim.loadAnimation(animIterator, "coin");

	currentAnimation = &anim;

	return true;
}