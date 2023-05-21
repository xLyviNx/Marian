#pragma once
#include "Base.h"
#include <allegro5/allegro.h>
#include <vector>
using namespace std;
class FloorObject : public Behaviour
{
	ALLEGRO_BITMAP* sprite;
	float* xof;
public:
	FloorObject(vector<Behaviour*>*, ALLEGRO_BITMAP*, float*);
	void Update();
};