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
    state = "waiting to roll";
}

void PlayerAvatar::doSomething()
{
    if(state == "waiting to roll")
    {
        if(getWorld()->getAction(playerNumber) == ACTION_ROLL)
        {
            int die_roll = randInt(1, 10);
            setTicksToMove(die_roll * 8);
            state = "walking";
        }
        else
            return;
    }
    if(state == "walking")
    {
        getNewDirection(getX(), getY());
        if(getWalkDir() == left)
            setDirection(left);
        else
            setDirection(right);

        moveAtAngle(getWalkDir(), 2);
        decTicksToMove();
       
        if(getTicksToMove() == 0)
           state = "waiting to roll";
    }
}

CoinSquare::CoinSquare(StudentWorld* sw, int imgID, int x, int y):Actor(sw, imgID, x, y, right, 1, 1.0)
{
    coinValue = 3;
}

void CoinSquare::doSomething()
{
    if(!isAlive())
        return;
}
