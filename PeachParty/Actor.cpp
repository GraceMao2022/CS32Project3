#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(StudentWorld* sw, int imgID, int x, int y, int dir = right, int depth = 0, double size = 1.0) : GraphObject(imgID, x, y, dir, depth, size), m_world(sw){}






CoinSquare::CoinSquare(StudentWorld* sw, int x, int y):Actor(sw, IID_BLUE_COIN_SQUARE, x, y, right, 1, 1.0)
{
    coins = 3;
}
