#pragma once
#include "Base.h"
#include <allegro5/allegro.h>
#include <vector>
using namespace std;
/**
 * @brief Klasa Podlogi.
 *
 * Klasa ta odpowiada za wyswietlanie podlogi.
 */
class FloorObject : public Behaviour
{
	ALLEGRO_BITMAP* sprite; /**< Wskaznik na grafike (bitmape) podlogi. */
	float* xof; /**< Wskaznik na offset kamery (w osi X). */
public:
	FloorObject(vector<Behaviour*>*, ALLEGRO_BITMAP*, float*);
	void Update();
};