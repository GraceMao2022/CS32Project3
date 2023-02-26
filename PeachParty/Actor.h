#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor: public GraphObject
{
public:
    Actor(StudentWorld* sw, int imgID, int x, int y, int dir, int depth, double size);
    virtual ~Actor(){}
    virtual void doSomething() = 0;
    bool isAlive() { return alive; }
    
protected:
    StudentWorld* getWorld() { return m_world; }
private:
    StudentWorld* m_world;
    bool alive;
};

class MovingActor: public Actor
{
public:
    MovingActor(StudentWorld* sw, int imgID, int x, int y, int dir, int depth, double size);
    virtual ~MovingActor(){}
    std::string getState() { return state; }
    //virtual void doSomething();
protected:
    int getTicksToMove() { return ticks_to_move; }
    void setTicksToMove(int ticks) { ticks_to_move = ticks; }
    void decTicksToMove() { ticks_to_move--; }
    int getWalkDir() { return walkDir; }
    void setWalkDir(int dir) { walkDir = dir; }
    void setState(std::string newState) { state = newState; }
    void getNewDirection(int x, int y);
private:
    int ticks_to_move;
    int walkDir;
    std::string state;
    bool canMove(int dir, int dist, int x, int y);
};

class PlayerAvatar: public MovingActor
{
public:
    PlayerAvatar(StudentWorld* sw, int imgID, int x, int y, int playerNum);
    ~PlayerAvatar(){}
    void doSomething();
    bool isPlayerOne() { return playerNumber == 1; }
    int getCoins() { return coins; }
    void setCoins(int amt) { coins = amt; }
private:
    int playerNumber;
    int coins;
    int stars;
};

class CoinSquare: public Actor
{
public:
    CoinSquare(StudentWorld* sw, int imgID, int x, int y, bool isBlue);
    ~CoinSquare(){}
    void doSomething();
private:
    int coinValue;
    bool peachIsNew;
    bool yoshiIsNew;
    
};

#endif // ACTOR_H_
