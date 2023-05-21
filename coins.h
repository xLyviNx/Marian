#pragma once
#include "Base.h"
#include <vector>
#include <allegro5/allegro.h>
using namespace std;
class coin
{
public:
	coin(float, float, ALLEGRO_BITMAP*);
	ALLEGRO_BITMAP* sprite;
	~coin();
	bool Collides(float, float);
	float x, y;
	void renderMe(float);

};