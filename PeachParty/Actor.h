#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor: public GraphObject
{
public:
    Actor(StudentWorld* sw, int imgID, int x, int y, int dir, int depth, double size);
    virtual ~Actor(){}// delete m_world; }
    virtual void doSomething() = 0;
    bool getIsAlive() { return isAlive; }
    
private:
    StudentWorld* m_world;
    bool isAlive;
};

class PlayerAvatar
{
    
};

class CoinSquare: public Actor
{
public:
    CoinSquare(StudentWorld* sw, int x, int y);
    ~CoinSquare(){}
    void doSomething(){}
private:
    int coins;
    
};

#endif // ACTOR_H_
