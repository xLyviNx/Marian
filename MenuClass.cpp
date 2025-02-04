#include "MenuClass.h"
#include "Base.h"
#include <allegro5/allegro_primitives.h>
#include "saveSystem.h"
#include <iostream>

/**
 * @brief Konstruktor Menu.
 *
 * @param bhvs Wskaznik na vector zawierajacy wskazniki Behaviours.
 * @param buttons Wskaznik na set zawierajacy aktualnie wcisniete przyciski.
 * @param pointer Podwojny wskaznik na obiekt Menu
 */
Menu::Menu(vector<Behaviour*>* bhvs, set<int>* buttons, Menu** pointer) : Behaviour(bhvs)
{
	al_get_mouse_state(&(this->mousestate));
	LoadedCoins = 0;
	this->menuPointer = pointer;
	this->TitleFont = al_load_ttf_font("Fonts/oldtimer.regular.ttf", 74, 0);
	this->ButtonsFont = al_load_ttf_font("Fonts/Qaz-Regular.TTF", 35, 0);
	this->otherFont = al_load_ttf_font("Fonts/Qaz-Regular.TTF", 18, 0);
	this->LoadedLevel = 1;
	this->Choice = 2;
	this->buttons = buttons;
	this->loadingdone = false;
	LoadedSpeed = 6.0;
	LoadedSSpeed = 0;
}
/**
 * @brief Funkcja Update, wywolywana co klatke.
 */
void Menu::Update()
{
	al_get_mouse_state(&(this->mousestate));
	if (this->hasFonts())
	{
		if (SaveSystem::Instance && !loadingdone)
		{
			LoadedLevel = -1;
			LoadedCoins = -1;
			LoadedSpeed = -1;
			LoadedSSpeed = -1;
			int newLoadedLevel = (*SaveSystem::Instance)->LoadLevel();
			int newLoadedCoins = (*SaveSystem::Instance)->LoadCoins();
			int n1 = (*SaveSystem::Instance)->LoadSpeed();
			float n2 = (*SaveSystem::Instance)->LoadShootSpeed();
			if (LoadedLevel != newLoadedLevel && LoadedCoins != newLoadedCoins)
			{
				LoadedLevel = newLoadedLevel;
				LoadedCoins = newLoadedCoins;
				LoadedSSpeed = n1;
				LoadedSpeed = n2;
				loadingdone = true;
			}
			else 
			{
				LoadedLevel = 1;
				LoadedCoins = 0;
			}
		}
		Choice = 0;
		int textWidth1 = al_get_text_width(ButtonsFont, "Nowa Gra");
		int textWidth2 = al_get_text_width(ButtonsFont, "Kontynuuj (Level X)");
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
		al_draw_textf(ButtonsFont, Choice == 2 ? onCol : offCol, WIDTH / 2, HEIGHT / 2 - 110, ALLEGRO_ALIGN_CENTER, "Kontynuuj (Level %d)", LoadedLevel);
		al_draw_text(ButtonsFont, Choice == 3 ? onCol : offCol, WIDTH / 2, HEIGHT / 2 - 70, ALLEGRO_ALIGN_CENTER, "Wyjdz");
		al_draw_textf(otherFont, onCol, WIDTH/2, HEIGHT/2 +100, ALLEGRO_ALIGN_CENTER, "Marian-Coins: %d", LoadedCoins);
		al_draw_text(otherFont, onCol, WIDTH/2, HEIGHT-60, ALLEGRO_ALIGN_CENTER, "Projekt PO1 - I1D14B (2023 SEM 2)");
		al_draw_text(otherFont, onCol, WIDTH/2, HEIGHT-40, ALLEGRO_ALIGN_CENTER, "Sygut Grzegorz, Strzepek Piotr, Synowiec Adrian");
		//al_draw_circle(mousestate.x, mousestate.y, 30, onCol, 5);

		
	}
}
/**
 * @brief Sprawdza czy obiekt Menu ma zaladowane czcionki.
 * @return Czy czcionki sa zaladowane.
 */
bool Menu::hasFonts()
{
	if (TitleFont && ButtonsFont)
	{
		return true;
	}
	return false;
}
/**
 * @brief Destruktor Menu, niszczacy czcionki i ustawiajaca *menuPointer na NULL
 */
Menu::~Menu()
{
	al_destroy_font(this->ButtonsFont);
	al_destroy_font(this->TitleFont);
	al_destroy_font(this->otherFont);
	*menuPointer = NULL;
	Behaviour::~Behaviour();
}

/**
 * @brief Konstruktor Pauzy
 * @param bhvs Wskaznik na vector zawierajacy wskazniki Behaviours.
 * @param buttons Wskaznik na set zawierajacy aktualnie wcisniete przyciski.
 * @param pointer Podwojny wskaznik na obiekt Pauzy.
 * @param dt Wskaznik na deltaTime (czas miedzy klatkami).
 * @param dead Wskaznik na wartosc logiczna mowiaca czy gracz zyje.
 */
Pause::Pause(vector<Behaviour*>* bhvs, set<int>* buttons, Pause** pointer, double* dt, bool* dead) : Behaviour(bhvs)
{
	this->pointer = pointer;
	this->TitleFont = al_load_ttf_font("Fonts/oldtimer.regular.ttf", 74, 0);
	this->uiFont = al_load_ttf_font("Fonts/Qaz-Regular.TTF", 20, 0);
	this->buttons = buttons;
	this->enablecd = 0;
	this->enabled = false;
	this->dT = dt;
	this->playerDied = dead;
}
/**
 * @brief Funkcja Update, wywolywana co klatke.
 */
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
			if ((*it) == ALLEGRO_KEY_ESCAPE && enablecd <= 0 && !*playerDied)
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
		if (*playerDied)
		{
			enabled = true;
		}
		if (enabled && playerDied)
		{
			ALLEGRO_COLOR white = al_map_rgb_f(1, 1, 1);
			if (!(*playerDied)) {
				al_draw_text(TitleFont, white, WIDTH / 2, HEIGHT / 2 - 200, ALLEGRO_ALIGN_CENTER, "PAUZA");
				al_draw_text(uiFont, white, WIDTH / 2, HEIGHT / 2 - 80, ALLEGRO_ALIGN_CENTER, "ESC by wrocic do GRY");
				al_draw_text(uiFont, white, WIDTH / 2, HEIGHT / 2 - 40, ALLEGRO_ALIGN_CENTER, "ENTER by wrocic do MENU");
			}
			else 
			{
				al_draw_text(TitleFont, white, WIDTH / 2, HEIGHT / 2 - 200, ALLEGRO_ALIGN_CENTER, "GAME OVER");
				al_draw_text(uiFont, white, WIDTH / 2, HEIGHT / 2 - 80, ALLEGRO_ALIGN_CENTER, "ENTER by wrocic do MENU");
			}
		}
	}
}
/**
 * @brief Sprawdza czy obiekt Pauzy ma zaladowane czcionki.
 * @return Czy czcionki sa zaladowane.
 */
bool Pause::hasFonts()
{
	if (TitleFont && uiFont)
	{
		return true;
	}
	return false;
}
/**
 * @brief Destruktor Pauzy, niszczacy czcionki i ustawiajaca *pointer na NULL
 */
Pause::~Pause()
{
	al_destroy_font(this->uiFont);
	al_destroy_font(this->TitleFont);
	*pointer = NULL;
	Behaviour::~Behaviour();
}