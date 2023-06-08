#include "Base.h"
#include <iostream>
using namespace std;
int GlobalAction = 0;


void Behaviour::Update() {};
void Behaviour::Start() {};

/**
 * @brief Konstruktor Behaviour.
 * @param bhvs Wskaznik na wektor behaviours.
 */
Behaviour::Behaviour(vector<Behaviour*>* bhvs)
{
    cout << "Constructed NEW Behaviour.\n";
    this->removing = false;
    this->vec = bhvs;
    if (this->vec != NULL)
    {
        (*(this->vec)).push_back(this);
    }
}
/**
 * @brief Destruktor Behaviour.
 */
Behaviour::~Behaviour()
{
    cout << "DESTRUCTION OF Behaviour.\n";
    if (this->vec != NULL)
    {
        auto it = find(vec->begin(), vec->end(), this);
        if (it != vec->end())
        {
            vec->erase(it);
        }
    }
}
