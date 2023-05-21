#include "floorO.h"

FloorObject::FloorObject(vector<Behaviour*>* vec, ALLEGRO_BITMAP* Sprite, float* Xof) : Behaviour(vec)
{
	sprite = Sprite;
	xof = Xof;
}
void FloorObject::Update()
{
	al_draw_bitmap(sprite, 0 - *xof, 0, 0);
}