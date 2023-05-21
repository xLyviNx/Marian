#include "MenuClass.h"
#include "Base.h"
#include <allegro5/allegro_primitives.h>
#include "saveSystem.h"
#include <iostream>
Menu::Menu(vector<Behaviour*>* bhvs, set<int>* buttons, Menu** pointer) : Behaviour(bhvs)
{
	al_get_mouse_state(&(this->mousestate));
	this->menuPointer = pointer;
	this->TitleFont = al_load_ttf_font("Fonts/oldtimer.regular.ttf", 74, 0);
	this->ButtonsFont = al_load_ttf_font("Fonts/Qaz-Regular.TTF", 35, 0);
	this->LoadedLevel = 1;
	this->Choice = 2;
	this->buttons = buttons;
	this->loadingdone = false;
}

void Menu::Update()
{
	al_get_mouse_state(&(this->mousestate));
	if (this->hasFonts())
	{
		if (SaveSystem::Instance && !loadingdone)
		{
			LoadedLevel = -1;
			int newLoadedLevel = (*SaveSystem::Instance)->LoadLevel();
			if (LoadedLevel != newLoadedLevel)
			{
				LoadedLevel = newLoadedLevel;
				loadingdone = true;
			}
			else {
				LoadedLevel = 1;
			}
		}
		Choice = 0;
		int textWidth1 = al_get_text_width(ButtonsFont, "Nowa Gra");
		int textWidth2 = al_get_text_width(ButtonsFont, "Kontynuuj (X)");
		int textWidth3 = al_get_text_width(ButtonsFont, "Wyjdz");
		float boundL = WIDTH / 2 - textWidth1 / 2;
		float boundR = WIDTH / 2 + textWidth1 / 2;
		float textV = HEIGHT / 2 - 132.5;
		float boundT = textV - 17.5;
		float boundB = textV + 17.5;
		//al_draw_rectangle(boundL, boundT, boundR, boundB, offCol, 3);
		if (Choice == 0)
		{
			if (mousestate.x > boundL && mousestate.x < boundR)
			{
				if (mousestate.y > boundT && mousestate.y < boundB)
				{
					Choice = 1;
				}
			}
		}
		if (Choice == 0)
		{
			boundL = WIDTH / 2 - textWidth2 / 2;
			boundR = WIDTH / 2 + textWidth2 / 2;
			boundT += 40;
			boundB += 40;

			if (mousestate.x > boundL && mousestate.x < boundR)
			{
				if (mousestate.y > boundT && mousestate.y < boundB)
				{
					Choice = 2;
				}
			}
		}
		if (Choice == 0)
		{

			boundL = WIDTH / 2 - textWidth3 / 2;
			boundR = WIDTH / 2 + textWidth3 / 2;
			boundT += 40;
			boundB += 40;
			if (mousestate.x > boundL && mousestate.x < boundR)
			{
				if (mousestate.y > boundT && mousestate.y < boundB)
				{
					Choice = 3;
				}
			}
		}
		if (al_mouse_button_down(&mousestate, 1))
		{
			if (Choice == 1)
			{
				GlobalAction = 1;
			}
			else if (Choice == 2)
			{
				GlobalAction = 2;
			}
			else if (Choice == 3)
			{
				GlobalAction = -1;
			}
		}
		al_clear_to_color(al_map_rgb(0, 100, 100));
		al_draw_text(TitleFont, al_map_rgb_f(1, 1, 1), WIDTH / 2, 30, ALLEGRO_ALIGN_CENTER, "Marian");
		ALLEGRO_COLOR offCol = al_map_rgb_f(0.5, 0.5, 0.7);
		ALLEGRO_COLOR onCol = al_map_rgb_f(1, 1, 1);
		al_draw_text(ButtonsFont, Choice == 1 ? onCol : offCol, WIDTH / 2, HEIGHT / 2 - 150, ALLEGRO_ALIGN_CENTER, "Nowa Gra");
		al_draw_textf(ButtonsFont, Choice == 2 ? onCol : offCol, WIDTH / 2, HEIGHT / 2 - 110, ALLEGRO_ALIGN_CENTER, "Kontynuuj (%d)", LoadedLevel);
		al_draw_text(ButtonsFont, Choice == 3 ? onCol : offCol, WIDTH / 2, HEIGHT / 2 - 70, ALLEGRO_ALIGN_CENTER, "Wyjdz");
		//al_draw_circle(mousestate.x, mousestate.y, 30, onCol, 5);

		
	}
}
bool Menu::hasFonts()
{
	if (TitleFont && ButtonsFont)
	{
		return true;
	}
	return false;
}
Menu::~Menu()
{
	*menuPointer = NULL;
	Behaviour::~Behaviour();
}


Pause::Pause(vector<Behaviour*>* bhvs, set<int>* buttons, Pause** pointer, double* dt) : Behaviour(bhvs)
{
	this->pointer = pointer;
	this->TitleFont = al_load_ttf_font("Fonts/oldtimer.regular.ttf", 74, 0);
	this->uiFont = al_load_ttf_font("Fonts/Qaz-Regular.TTF", 20, 0);
	this->buttons = buttons;
	this->enablecd = 0;
	this->enabled = false;
	this->dT = dt;
}

void Pause::Update()
{
	if (this->hasFonts())
	{
		if (enablecd > 0)
		{
			enablecd -= *dT;
		}
		for (set<int>::iterator it = (*buttons).begin(); it != (*buttons).end(); it++)
		{
			if ((*it) == ALLEGRO_KEY_ESCAPE && enablecd<=0)
			{
				enablecd = 0.3;
				enabled = !enabled;
			}
			else if ((*it) == ALLEGRO_KEY_ENTER)
			{
				if (enabled)
				{
					GlobalAction = 4;
				}
			}
		}
		if (enabled)
		{
			ALLEGRO_COLOR white = al_map_rgb_f(1, 1, 1);

			al_draw_text(TitleFont, white, WIDTH / 2, HEIGHT/2-200, ALLEGRO_ALIGN_CENTER, "PAUZA");
			al_draw_text(uiFont, white, WIDTH / 2, HEIGHT / 2 - 80, ALLEGRO_ALIGN_CENTER, "ESC by wrocic do GRY");
			al_draw_text(uiFont, white, WIDTH / 2, HEIGHT / 2 - 40, ALLEGRO_ALIGN_CENTER, "ENTER by wrocic do MENU");
		}
	}
}
bool Pause::hasFonts()
{
	if (TitleFont && uiFont)
	{
		return true;
	}
	return false;
}
Pause::~Pause()
{
	*pointer = NULL;
	Behaviour::~Behaviour();
}