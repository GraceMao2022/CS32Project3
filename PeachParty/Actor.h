#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor: public GraphObject
{
public:
    Actor(StudentWorld* sw, int imgID, int x, int y, int dir, int depth, double size);
    virtual void doSomething() = 0;
    void setIsAlive(bool isAlive) { alive = isAlive; }
    bool isAlive() const { return alive; }
    virtual bool isSquare() const = 0;
    virtual bool isImpactable() const = 0;
    bool isOn(int x, int y) const;
    bool isPeachNew() const { return peachIsNew; }
    void setPeachIsNew(bool peachNew) { peachIsNew = peachNew; }
    bool isYoshiNew() const { return yoshiIsNew; }
    void setYoshiIsNew(bool yoshiNew) { yoshiIsNew = yoshiNew; }
    
protected:
    StudentWorld* getWorld() { return m_world; }
private:
    StudentWorld* m_world; //pointer to world that actor is in
    bool alive; //if actor is alive or not
    bool peachIsNew; //if peach has not collided with this actor yet
    bool yoshiIsNew; //if yoshi has not collided with this actor yet
};

class MovingActor: public Actor
{
public:
    MovingActor(StudentWorld* sw, int imgID, int x, int y);
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
    int ticks_to_move; //stores the ticks left for actor to move
    int walkDir; //stores movement direction
    std::string state; //stores current state of actor
};

class PlayerAvatar: public MovingActor
{
public:
    PlayerAvatar(StudentWorld* sw, int imgID, int x, int y, int playerNum);
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
    bool playerHasVortex() const { return hasVortex; }
    bool isImpactable() const { return false; }
    
private:
    int playerNumber; //player 1 = peach, player 2 = yoshi
    int coins; //stores number of coins player has
    int stars; //stores number of stars player has
    bool hasVortex; //stores if player currently has vortex or not
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
    int pauseCounter; //counter that ticks down when enemy is paused
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
};

class CoinSquare: public Square
{
public:
    CoinSquare(StudentWorld* sw, int imgID, int x, int y, bool isBlue);
    void doAction(PlayerAvatar* playerPtr);
private:
    int coinValue; //3 or -3 depending on the coin
    bool blue; //true if coin is blue, false if red
};

class StarSquare: public Square
{
public:
    StarSquare(StudentWorld* sw, int x, int y);
    void doAction(PlayerAvatar* playerPtr);
};

class DirectionalSquare: public Square
{
public:
    DirectionalSquare(StudentWorld* sw, int x, int y, int dir);
    void doAction(PlayerAvatar* playerPtr);
private:
    int forcingDir; //direction to force the player
};

class BankSquare: public Square
{
public:
    BankSquare(StudentWorld* sw, int x, int y);
    void doAction(PlayerAvatar* playerPtr);
};

class EventSquare: public Square
{
public:
    EventSquare(StudentWorld* sw, int x, int y);
    void doAction(PlayerAvatar* playerPtr);
};

class DroppingSquare: public Square
{
public:
    DroppingSquare(StudentWorld* sw, int x, int y);
    void doAction(PlayerAvatar* playerPtr);
};

#endif // ACTOR_H_
