#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor: public GraphObject
{
public:
    Actor(StudentWorld* sw, int imgID, int x, int y, int dir, int depth, double size);
    //virtual ~Actor(){}
    virtual void doSomething() = 0;
    bool isAlive() const { return alive; }
    virtual bool isSquare() const = 0;
    virtual bool isImpactable() const = 0;
    
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
    //virtual ~MovingActor(){}
    std::string getState() const { return state; }
    void setWalkDir(int dir) { walkDir = dir; }
protected:
    int getTicksToMove() const { return ticks_to_move; }
    void setTicksToMove(int ticks) { ticks_to_move = ticks; }
    void decTicksToMove() { ticks_to_move--; }
    int getWalkDir() const { return walkDir; }
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
    //~PlayerAvatar(){}
    void doSomething();
    bool isPlayerOne() const { return playerNumber == 1; }
    int getCoins() const { return coins; }
    void setCoins(int amt) { coins = amt; }
    int getStars() const { return stars; }
    void setStars(int amt) { stars = amt; }
    bool isSquare() const { return false; }
    void teleportToRandomSquare();
    void swapAttributesWithOther(PlayerAvatar* other);
    void setHasVortex(bool hasVor) { hasVortex = hasVor; }
    bool playerHasVortex() { return hasVortex; }
    bool isImpactable() const { return false; }
    
private:
    int playerNumber;
    int coins;
    int stars;
    bool hasVortex;
};

class Bowser: public MovingActor
{
public:
    Bowser(StudentWorld* sw, int x, int y);
    bool isImpactable() const { return true; }
private:
    int travelDist;
    int pauseCounter;
};

class Square: public Actor
{
public:
    Square(StudentWorld* sw, int imgID, int x, int y, int dir);
    void doSomething();
protected:
    bool isPeachNew() { return peachIsNew; }
    void setPeachIsNew(bool peachNew) { peachIsNew = peachNew; }
    bool isYoshiNew() { return yoshiIsNew; }
    void setYoshiIsNew(bool yoshiNew) { yoshiIsNew = yoshiNew; }
    bool isOn(int x, int y);
    bool isSquare() const { return true; }
    virtual void doAction(PlayerAvatar* playerPtr) = 0;
    bool isImpactable() const { return false; }
private:
    bool peachIsNew;
    bool yoshiIsNew;
};

class CoinSquare: public Square
{
public:
    CoinSquare(StudentWorld* sw, int imgID, int x, int y, bool isBlue);
    //~CoinSquare(){}
    void doAction(PlayerAvatar* playerPtr);
private:
    int coinValue;
    bool blue;
    
};

class StarSquare: public Square
{
public:
    StarSquare(StudentWorld* sw, int x, int y);
    //~StarSquare(){}
    void doAction(PlayerAvatar* playerPtr);
private:
};

class DirectionalSquare: public Square
{
public:
    DirectionalSquare(StudentWorld* sw, int x, int y, int dir);
   // ~DirectionalSquare(){}
    void doAction(PlayerAvatar* playerPtr);
private:
    int forcingDir;
};

class BankSquare: public Square
{
public:
    BankSquare(StudentWorld* sw, int x, int y);
    //~BankSquare(){}
    void doAction(PlayerAvatar* playerPtr);
private:
};

class EventSquare: public Square
{
public:
    EventSquare(StudentWorld* sw, int x, int y);
   // ~EventSquare(){}
    void doAction(PlayerAvatar* playerPtr);
private:
};

class DroppingSquare: public Square
{
public:
    DroppingSquare(StudentWorld* sw, int x, int y);
    //~DroppingSquare(){}
    void doAction(PlayerAvatar* playerPtr);
private:
};

#endif // ACTOR_H_
