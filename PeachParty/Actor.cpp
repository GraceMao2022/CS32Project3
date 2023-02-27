#include "Actor.h"
#include "StudentWorld.h"
#include "Board.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(StudentWorld* sw, int imgID, int x, int y, int dir = right, int depth = 0, double size = 1.0) : GraphObject(imgID, x, y, dir, depth, size), m_world(sw)
{
    alive = true;
}

MovingActor::MovingActor(StudentWorld* sw, int imgID, int x, int y, int dir, int depth, double size) : Actor(sw, imgID, x, y, dir, depth, size)
{
    ticks_to_move = 0;
    walkDir = right;
}

void MovingActor::getNewDirection(int x, int y)
{
    switch(walkDir)
    {
        case left:
        {
            if(!canMove(left, 2, x, y))
            {
                //move up instead if possible
                if(canMove(up, 2, x, y))
                    walkDir = up;
                else if(canMove(down, 2, x, y))
                    walkDir = down;
            }
            break;
        }
        case right:
        {
            if(!canMove(right, 2, x, y))
            {
                //move up instead if possible
                if(canMove(up, 2, x, y))
                    walkDir = up;
                else if(canMove(down, 2, x, y))
                    walkDir = down;
            }
            break;
        }
        case up:
        {
            if(!canMove(up, 2, x, y))
            {
                //move up instead if possible
                if(canMove(right, 2, x, y))
                    walkDir = right;
                else if(canMove(left, 2, x, y))
                    walkDir = left;
            }
            break;
        }
        case down:
        {
            if(!canMove(down, 2, x, y))
            {
                //move up instead if possible
                if(canMove(right, 2, x, y))
                    walkDir = right;
                else if(canMove(left, 2, x, y))
                    walkDir = left;
            }
            break;
        }
    }
}

bool MovingActor::canMove(int dir, int dist, int x, int y)
{
    if(dir == left)
    {
        //if it goes off screen or on a tile with nothing
        if(x-dist < 0 || getWorld()->getActorAt((x-dist)/SPRITE_WIDTH, y/SPRITE_HEIGHT) == Board::empty)
            return false;
        return true;
    }
    else if(dir == right)
    {
        if(x+dist + SPRITE_WIDTH >= VIEW_WIDTH || getWorld()->getActorAt(x/SPRITE_WIDTH+1, y/SPRITE_HEIGHT) == Board::empty)
        {
            return false;
        }
        return true;
    }
    else if(dir == up)
    {
        if(y+dist + SPRITE_HEIGHT >= VIEW_HEIGHT || getWorld()->getActorAt(x/SPRITE_WIDTH, y/SPRITE_HEIGHT+1) == Board::empty)
            return false;
        return true;
    }
    else
    {
        if(y-dist <0 || getWorld()->getActorAt(x/SPRITE_WIDTH, (y-dist)/SPRITE_HEIGHT) == Board::empty)
            return false;
        return true;
    }
}

void MovingActor::updateSpriteDirection()
{
    if(walkDir == left)
        setDirection(left);
    else
        setDirection(right);
}

// finds all possible directions the actor can move and chooses one randomly
int MovingActor::chooseRandomWalkDir()
{
    std::vector<int> possibleDirections; //stores all possible directions actor can move to
    if(canMove(right, 2, getX(), getY()))
        possibleDirections.push_back(right);
    if(canMove(left, 2, getX(), getY()))
        possibleDirections.push_back(left);
    if(canMove(up, 2, getX(), getY()))
        possibleDirections.push_back(up);
    if(canMove(down, 2, getX(), getY()))
        possibleDirections.push_back(down);
    
    int randDir = randInt(0, (int)possibleDirections.size() - 1);
    
    return possibleDirections[randDir];
}

PlayerAvatar::PlayerAvatar(StudentWorld* sw, int imgID, int x, int y, int playerNum):MovingActor(sw, imgID, x, y, right, 0, 1.0)
{
    playerNumber = playerNum;
    setState("waiting to roll");
    coins = 0;
    stars = 0;
}

void PlayerAvatar::doSomething()
{
    if(getState() == "waiting to roll")
    {
        if(!canMove(getWalkDir(), 2, getX(), getY()))
        {
            setWalkDir(chooseRandomWalkDir());
            updateSpriteDirection();
        }
        switch (getWorld()->getAction(playerNumber))
        {
            case ACTION_ROLL:
            {
                int die_roll = randInt(1, 10);
                setTicksToMove(die_roll * 8);
                setState("walking");
                break;
            }
            default:
                return;
        }
    }
    if(getState() == "walking")
    {
        getNewDirection(getX(), getY());

        updateSpriteDirection();
        
        moveAtAngle(getWalkDir(), 2);
        decTicksToMove();
       
        if(getTicksToMove() == 0)
           setState("waiting to roll");
    }
}
Square::Square(StudentWorld* sw, int imgID, int x, int y, int dir):Actor(sw, imgID, x, y, dir, 1, 1.0)
{
    peachIsNew = false;
    yoshiIsNew = false;
}

bool Square::isOn(int x, int y)
{
    if(x == getX() && y == getY())
        return true;
    return false;
}

CoinSquare::CoinSquare(StudentWorld* sw, int imgID, int x, int y, bool isBlue):Square(sw, imgID, x, y, right)
{
    blue = isBlue;
    if(isBlue)
        coinValue = 3;
    else
        coinValue = -3;
}

void CoinSquare::doSomething()
{
    if(!isAlive())
        return;
   if(isOn(getWorld()->getPeach()->getX(), getWorld()->getPeach()->getY()))
    {
        if(getWorld()->getPeach()->getState() == "waiting to roll" && isPeachNew())
        {
            getWorld()->getPeach()->setCoins(getWorld()->getPeach()->getCoins()+coinValue);
            //if subtracted coins past 0
            if(getWorld()->getPeach()->getCoins() < 0)
                getWorld()->getPeach()->setCoins(0);
            
            //play sound give coin
            if(isBlue())
                getWorld()->playSound(SOUND_GIVE_COIN);
            //play sound take coin
            else if(!isBlue())
                getWorld()->playSound(SOUND_TAKE_COIN);
            
            setPeachIsNew(false);
        }
    }
    else
        setPeachIsNew(true);
    
    if(isOn(getWorld()->getYoshi()->getX(), getWorld()->getYoshi()->getY()))
     {
         if(getWorld()->getYoshi()->getState() == "waiting to roll" && isYoshiNew())
         {
             getWorld()->getYoshi()->setCoins(getWorld()->getYoshi()->getCoins()+coinValue);
             //if subtracted coins past 0
             if(getWorld()->getYoshi()->getCoins() < 0)
                 getWorld()->getYoshi()->setCoins(0);
             
             //play sound give coin
             if(isBlue())
                 getWorld()->playSound(SOUND_GIVE_COIN);
             //play sound take coin
             else if(!isBlue())
                 getWorld()->playSound(SOUND_TAKE_COIN);
             
             setYoshiIsNew(false);
         }
     }
     else
         setYoshiIsNew(true);
}

StarSquare::StarSquare(StudentWorld* sw, int imgID, int x, int y):Square(sw, imgID, x, y)
{
    
}

void StarSquare::doSomething()
{
    if(isOn(getWorld()->getPeach()->getX(), getWorld()->getPeach()->getY()) && isPeachNew())
    {
        if(getWorld()->getPeach()->getCoins() < 20)
         return;
        
        getWorld()->getPeach()->setCoins(getWorld()->getPeach()->getCoins()-20);
        getWorld()->getPeach()->setStars(getWorld()->getPeach()->getStars()+1);
        getWorld()->playSound(SOUND_GIVE_STAR);

        setPeachIsNew(false);
    }
    else
        setPeachIsNew(true);

    if(isOn(getWorld()->getYoshi()->getX(), getWorld()->getYoshi()->getY()) && isYoshiNew())
    {
        if(getWorld()->getYoshi()->getCoins() < 20)
         return;
        
        getWorld()->getYoshi()->setCoins(getWorld()->getYoshi()->getCoins()-20);
        getWorld()->getYoshi()->setStars(getWorld()->getYoshi()->getStars()+1);
        getWorld()->playSound(SOUND_GIVE_STAR);

        setYoshiIsNew(false);
    }
    else
        setYoshiIsNew(true);
}
