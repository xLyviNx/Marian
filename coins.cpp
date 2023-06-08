#include "coins.h"
#include <iostream>

/**
 * @brief Konstruktor coina.
 * @param X Wspolrzedna X.
 * @param Y Wspolrzedna Y.
 * @param sprite Sprite coina (wskaznik na bitmape).
 */
coin::coin(float X, float Y, ALLEGRO_BITMAP* sprite)
{
	this->x = X;
	this->y = Y;
	this->sprite = sprite;
}
/**
 * @brief Sprawdza czy coin koliduje z graczem.
 * @param X Wspolrzedna X.
 * @param Y Wspolrzedna Y.
 * @return Czy coin koliduje z danymi koordynatami.
 */
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
/**
 * @brief Wyswietla coina na ekranie.
 * @param cam_x_offset Offset kamery na osi X.
 */
void coin::renderMe(float cam_x_offset)
{
	if (this->sprite) {
		al_draw_scaled_bitmap(this->sprite, 0, 0, 100, 100, this->x - cam_x_offset - 25, this->y - 25, 50, 50, 0);
	}
}