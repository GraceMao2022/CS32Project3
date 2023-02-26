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
        switch (getWorld()->getAction(playerNumber))
        {
            case ACTION_NONE:
                return;
            case ACTION_ROLL:
            {
                int die_roll = randInt(1, 10);
                setTicksToMove(die_roll * 8);
                setState("walking");
                break;
            }
        }
    }
    if(getState() == "walking")
    {
        getNewDirection(getX(), getY());
        if(getWalkDir() == left)
            setDirection(left);
        else
            setDirection(right);

        moveAtAngle(getWalkDir(), 2);
        decTicksToMove();
       
        if(getTicksToMove() == 0)
           setState("waiting to roll");
    }
}

CoinSquare::CoinSquare(StudentWorld* sw, int imgID, int x, int y, bool isBlue):Actor(sw, imgID, x, y, right, 1, 1.0)
{
    if(isBlue)
        coinValue = 3;
    else
        coinValue = -3;
    
    peachIsNew = false;
    yoshiIsNew = false;
}

void CoinSquare::doSomething()
{
    if(!isAlive())
        return;
   if(getWorld()->getPeach()->getX() == getX() && getWorld()->getPeach()->getY() == getY())
    {
        if(getWorld()->getPeach()->getState() == "waiting to roll" && peachIsNew)
        {
            getWorld()->getPeach()->setCoins(getWorld()->getPeach()->getCoins()+coinValue);
            //if subtracted coins past 0
            if(getWorld()->getPeach()->getCoins() < 0)
                getWorld()->getPeach()->setCoins(0);
            
            //play sound give coin
            if(coinValue > 0)
                getWorld()->playSound(SOUND_GIVE_COIN);
            //play sound take coin
            else if(coinValue < 0)
                getWorld()->playSound(SOUND_TAKE_COIN);
            
            peachIsNew = false;
        }
    }
    else
        peachIsNew = true;
    
    if(getWorld()->getYoshi()->getX() == getX() && getWorld()->getYoshi()->getY() == getY())
     {
         if(getWorld()->getYoshi()->getState() == "waiting to roll" && yoshiIsNew)
         {
             getWorld()->getYoshi()->setCoins(getWorld()->getYoshi()->getCoins()+coinValue);
             //if subtracted coins past 0
             if(getWorld()->getYoshi()->getCoins() < 0)
                 getWorld()->getYoshi()->setCoins(0);
             
             //play sound give coin
             if(coinValue > 0)
                 getWorld()->playSound(SOUND_GIVE_COIN);
             //play sound take coin
             else if(coinValue < 0)
                 getWorld()->playSound(SOUND_TAKE_COIN);
             
             yoshiIsNew = false;
         }
     }
     else
         yoshiIsNew = true;
}
