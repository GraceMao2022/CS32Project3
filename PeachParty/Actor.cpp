#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(StudentWorld* sw, int imgID, int x, int y) : GraphObject(imgID, x, y), m_world(sw){}






CoinSquare::CoinSquare(StudentWorld* sw, int x, int y):Actor(sw, IID_BLUE_COIN_SQUARE, x, y)
{
    coins = 3;
}
