#include "floorO.h"
/**
 * @brief Konstruktor podlogi.
 *
 * @param vec Wskaznik na vector zawierajacy wskazniki Behaviours.
 * @param Sprite Wskaznik na grafike (bitmape) podlogi.
 * @param Xof Wskaznik na offset kamery (na osi X)
 */
FloorObject::FloorObject(vector<Behaviour*>* vec, ALLEGRO_BITMAP* Sprite, float* Xof) : Behaviour(vec)
{
	sprite = Sprite;
	xof = Xof;
}
/**
 * @brief Funkcja Update, wywolywana co klatke.
 */
void FloorObject::Update()
{
	al_draw_bitmap(sprite, 0 - *xof, 0, 0);
}