#pragma once
#include "Base.h"
#include <vector>
#include <set>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

using namespace std;
/**
 * @brief Klasa Menu Glownego.
 *
 * Klasa ta odpowiada za wyswietlanie i operacje na menu glownym.
 */
class Menu : public Behaviour
{
	set<int>* buttons;/**< Wskaznik na set wcisnietych przyciskow. */
	ALLEGRO_FONT* TitleFont; /**< Wskaznik na czcionke tytulu gry. */
	ALLEGRO_FONT* otherFont; /**< Wskaznik na inna czcionke. */
	ALLEGRO_FONT* ButtonsFont; /**< Wskaznik na czcionke przyciskow. */
	ALLEGRO_MOUSE_STATE mousestate; /**< Stan myszy. */
	bool loadingdone; /**< Wartosc logiczna - czy zakonczono ladowanie. */
	Menu** menuPointer; /**< Podwojny wskaznik na menu (przy tworzeniu menu automatycznie jest ustawiana wartosc *menuPointer). */
public:
	int LoadedLevel; /**< Zaladowany poziom. */
	int LoadedCoins; /**< Zaladowana ilosc coinow. */
	int LoadedSpeed; /**< Zaladowana predkosc gracza. */
	float LoadedSSpeed; /**< Zaladowana szybkostrzelnosc gracza. */
	int Choice; /**< Aktualnie wybierany przycisk Menu Glownego. */
	void Update(); 
	bool hasFonts();
	Menu(vector<Behaviour*>*, set<int>*, Menu**);
	~Menu();
};

/**
 * @brief Klasa Pauzy.
 *
 * Klasa ta odpowiada za Pauze.
 */
class Pause : public Behaviour
{
	Pause** pointer;  /**< Podwojny wskaznik na obiekt Pauzy*/
	set<int>* buttons;/**< Wskaznik na set wcisnietych przyciskow. */
	ALLEGRO_FONT* TitleFont; /**< Wskaznik na czcionke tytulu gry. */
	ALLEGRO_FONT* uiFont; /**< Wskaznik na inna czcionke. */
	float enablecd;/**< Pozostaly czas do ponownego przelaczenia pauzy. */
	bool* playerDied;/**< Wskaznik na wartosc logiczna, czy gracz umarl. */
	double* dT; /**< Wskaznik na deltaTime, czyli czas miedzy klatkami. */
public:
	bool enabled; /**< Czy pauza jest aktywna. */
	void Update();
	bool hasFonts();
	Pause(vector<Behaviour*>*, set<int>*, Pause**, double*, bool*);
	~Pause();
};