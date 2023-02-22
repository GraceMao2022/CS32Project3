#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class BaseActor: public GraphObject
{
    //GraphObject(int imageID, int startX, int startY, int dir = right, int depth = 0, double size = 1.0)
    public:
    BaseActor(int imageID, int startX, int startY):GraphObject(imageID, startX, startY)
    {
        
    }
    virtual void doSomething() = 0;
};

class PlayerAvatar
{
    
};

class CoinSquare
{
    
};

#endif // ACTOR_H_
