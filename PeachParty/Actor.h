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
    bool isAlive() const { return alive; }
    
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
    std::string getState() const { return state; }
protected:
    int getTicksToMove() const { return ticks_to_move; }
    void setTicksToMove(int ticks) { ticks_to_move = ticks; }
    void decTicksToMove() { ticks_to_move--; }
    int getWalkDir() const { return walkDir; }
    void setWalkDir(int dir) { walkDir = dir; }
    void setState(std::string newState) { state = newState; }
    void getNewDirection(int x, int y);
    bool canMove(int dir, int dist, int x, int y);
    void updateSpriteDirection();
    int chooseRandomWalkDir();
private:
    int ticks_to_move;
    int walkDir;
    std::string state;
};

class PlayerAvatar: public MovingActor
{
public:
    PlayerAvatar(StudentWorld* sw, int imgID, int x, int y, int playerNum);
    ~PlayerAvatar(){}
    void doSomething();
    bool isPlayerOne() const { return playerNumber == 1; }
    int getCoins() const { return coins; }
    void setCoins(int amt) { coins = amt; }
    int getStars() const { return stars; }
    void setStars(int amt) { stars = amt; }
private:
    int playerNumber;
    int coins;
    int stars;
};

class Square: public Actor
{
public:
    Square(StudentWorld* sw, int imgID, int x, int y, int dir);
    virtual ~Square();
protected:
    bool isPeachNew() { return peachIsNew; }
    void setPeachIsNew(bool peachNew) { peachIsNew = peachNew; }
    bool isYoshiNew() { return yoshiIsNew; }
    void setYoshiIsNew(bool yoshiNew) { yoshiIsNew = yoshiNew; }
    bool isOn(int x, int y);
private:
    bool peachIsNew;
    bool yoshiIsNew;
};

class CoinSquare: public Square
{
public:
    CoinSquare(StudentWorld* sw, int imgID, int x, int y, bool isBlue);
    ~CoinSquare(){}
    void doSomething();
private:
    int coinValue;
    bool blue;
    bool isBlue() { return blue; }
    
};

class StarSquare: public Square
{
public:
    StarSquare(StudentWorld* sw, int imgID, int x, int y);
    ~StarSquare(){}
    void doSomething();
private:
   
    
};

#endif // ACTOR_H_
