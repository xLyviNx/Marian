#pragma once
#include "Base.h"
#include <vector>
#include <set>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

using namespace std;
class Menu : public Behaviour
{
	set<int>* buttons;
	ALLEGRO_FONT* TitleFont;
	ALLEGRO_FONT* ButtonsFont;
	ALLEGRO_MOUSE_STATE mousestate;
	bool loadingdone;
	Menu** menuPointer;
public:
	int LoadedLevel;
	int Choice;
	void Update();
	bool hasFonts();
	Menu(vector<Behaviour*>*, set<int>*, Menu**);
	~Menu();
};

class Pause : public Behaviour
{
	Pause** pointer;
	set<int>* buttons;
	ALLEGRO_FONT* TitleFont;
	ALLEGRO_FONT* uiFont;
	float enablecd;
	double* dT;
public:
	bool enabled;
	void Update();
	bool hasFonts();
	Pause(vector<Behaviour*>*, set<int>*, Pause**, double*);
	~Pause();
};