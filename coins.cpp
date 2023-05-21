#include "coins.h"
#include <iostream>


coin::coin(float X, float Y, ALLEGRO_BITMAP* sprite)
{
	this->x = X;
	this->y = Y;
	this->sprite = sprite;
}

coin::~coin()
{

}
bool coin::Collides(float X, float Y)
{
	if (X >= x - 25 && X <= x + 25)
	{
		if (Y >= y - 25 && Y <= y + 25)
		{
			return true;
		}
	}
	return false;
}
void coin::renderMe(float cam_x_offset)
{
	if (this->sprite) {
		al_draw_scaled_bitmap(this->sprite, 0, 0, 100, 100, this->x - cam_x_offset - 25, this->y - 25, 50, 50, 0);
	}
}