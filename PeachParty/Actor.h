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
    void setIsAlive(bool isAlive) { alive = isAlive; }
    bool isAlive() const { return alive; }
    virtual bool isSquare() const = 0;
    virtual bool isImpactable() const = 0;
    bool isOn(int x, int y);
    bool isPeachNew() { return peachIsNew; }
    void setPeachIsNew(bool peachNew) { peachIsNew = peachNew; }
    bool isYoshiNew() { return yoshiIsNew; }
    void setYoshiIsNew(bool yoshiNew) { yoshiIsNew = yoshiNew; }
    
protected:
    StudentWorld* getWorld() { return m_world; }
private:
    StudentWorld* m_world;
    bool alive;
    bool peachIsNew;
    bool yoshiIsNew;
};

class MovingActor: public Actor
{
public:
    MovingActor(StudentWorld* sw, int imgID, int x, int y);
    //virtual ~MovingActor(){}
    std::string getState() const { return state; }
    void setWalkDir(int dir) { walkDir = dir; }
    bool isSquare() const { return false; }
    int getTicksToMove() const { return ticks_to_move; }
    void teleportToRandomSquare();
protected:
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
    void swapStars(PlayerAvatar* playerPtr);
    void swapCoins(PlayerAvatar* playerPtr);
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

class Enemy: public MovingActor
{
public:
    Enemy(StudentWorld* sw, int imgID, int x, int y);
    bool isImpactable() const { return true; }
    void doSomething();
    virtual void doAction(PlayerAvatar* playerPtr) = 0;
    virtual bool canMakeDroppingSquare() const = 0;
    void doImpactedAction();
private:
    int travelDist;
    int pauseCounter;
};

class Bowser: public Enemy
{
public:
    Bowser(StudentWorld* sw, int x, int y);
    void doAction(PlayerAvatar* playerPtr);
    bool canMakeDroppingSquare() const { return true; }
};

class Boo: public Enemy
{
public:
    Boo(StudentWorld* sw, int x, int y);
    void doAction(PlayerAvatar* playerPtr);
    bool canMakeDroppingSquare() const { return false; }
};

class Vortex: public MovingActor
{
public:
    Vortex(StudentWorld* sw, int x, int y);
    bool isImpactable() const { return false; }
    void doSomething();
private:
};

class Square: public Actor
{
public:
    Square(StudentWorld* sw, int imgID, int x, int y, int dir);
    void doSomething();
protected:
    bool isSquare() const { return true; }
    virtual void doAction(PlayerAvatar* playerPtr) = 0;
    bool isImpactable() const { return false; }
private:
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
