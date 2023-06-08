#pragma once
#include <vector>
/**
 * @brief Wysokosc okna.
 */
#define HEIGHT 720 
 /**
  * @brief Szerokosc okna.
  */
#define WIDTH 1280
/**
 * @brief Globalna akcja odpowiada za wywolywanie konkretnych zachowan gry z kazdego poziomu kodu.
 */
extern int GlobalAction;
/**
 * @brief Klasa Behaviour.
 * 
 * Klasa ta jest dziediczona przez wiekszosc klas w grze. Dzieki niej w latwy sposob mozna zaimplementowac funkcje wywolywane co klatke, oraz latwiej zarzadzac obiektami w grze.
 */
class Behaviour
{
protected:
     std::vector<Behaviour*>* vec; /**< Wskaznik na wektor (liste) wszystkich Behaviours. */
public:
    bool removing;/**< Czy behaviour jest usuwane. */
    virtual void Update(); /**< Metoda wywolywana co klatke. */
    virtual void Start(); /**< Metoda ktora moze byc wywolana po konstruktorze. */
    //bool dontDestroy;
    Behaviour(std::vector<Behaviour*>*);
    virtual ~Behaviour();
};