#pragma once
#include <vector>
#define HEIGHT 720 
#define WIDTH 1280

extern int GlobalAction;

class Behaviour
{
protected:
     std::vector<Behaviour*>* vec;
public:
    bool removing;
    virtual void Update();
    virtual void Start();
    //bool dontDestroy;
    Behaviour(std::vector<Behaviour*>*);
    virtual ~Behaviour();
};