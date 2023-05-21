#include "Base.h"
#include <iostream>
using namespace std;
int GlobalAction = 0;


void Behaviour::Update() {};
void Behaviour::Start() {};
Behaviour::Behaviour(vector<Behaviour*>* bhvs)
{
    cout << "Constructed NEW Behaviour.\n";
    //this->dontDestroy = nodestroying;
    this->removing = false;
    this->vec = bhvs;
    if (this->vec != NULL)
    {
        (*(this->vec)).push_back(this);
    }
}
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
