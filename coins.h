#pragma once
#include "Base.h"
#include <vector>
#include <allegro5/allegro.h>
using namespace std;
/**
 * @brief Klasa coina.
 * 
 * Sa to punkty ktore gracz moze zdobywac, by kupowac ulepszenia.
 */
class coin
{
public:
	coin(float, float, ALLEGRO_BITMAP*);
	ALLEGRO_BITMAP* sprite; /**< Sprite coina (wskaznik na bitmape). */
	bool Collides(float, float);
	float x; /**< Pozycja na osi X coina na mapie. */
	float y; /**< Pozycja na osi Y coina na mapie. */
	void renderMe(float);

};