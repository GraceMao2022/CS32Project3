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
        possibleDirections.push_back(0);
    if(canMove(left, 2, getX(), getY()))
        possibleDirections.push_back(180);
    if(canMove(up, 2, getX(), getY()))
        possibleDirections.push_back(90);
    if(canMove(down, 2, getX(), getY()))
        possibleDirections.push_back(270);
    
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
                //setTicksToMove(8);
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
        {
            getNewDirection(getX(), getY()); //to get new direction if character is at a turn and is stopping there
            setState("waiting to roll");
        }
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

void Square::doSomething()
{
    //peach
    if(isOn(getWorld()->getPeach()->getX(), getWorld()->getPeach()->getY()))
    {
         if(isPeachNew())
         {
             doAction(getWorld()->getPeach());
             setPeachIsNew(false);
         }
     }
     else
         setPeachIsNew(true);
    
    //yoshi
    if(isOn(getWorld()->getYoshi()->getX(), getWorld()->getYoshi()->getY()))
    {
         if(isYoshiNew())
         {
             doAction(getWorld()->getYoshi());
             setYoshiIsNew(false);
         }
     }
     else
         setYoshiIsNew(true);
}

CoinSquare::CoinSquare(StudentWorld* sw, int imgID, int x, int y, bool isBlue):Square(sw, imgID, x, y, right)
{
    blue = isBlue;
    if(isBlue)
        coinValue = 3;
    else
        coinValue = -3;
}

void CoinSquare::doAction(PlayerAvatar* playerPtr)
{
    if(!isAlive())
        return;
    if(playerPtr->getState() == "waiting to roll")
    {
        playerPtr->setCoins(playerPtr->getCoins()+coinValue);
        //if subtracted coins past 0
        if(playerPtr->getCoins() < 0)
            playerPtr->setCoins(0);
        
        //play sound give coin
        if(blue)
            getWorld()->playSound(SOUND_GIVE_COIN);
        //play sound take coin
        else if(!blue)
            getWorld()->playSound(SOUND_TAKE_COIN);
    }
}

StarSquare::StarSquare(StudentWorld* sw, int x, int y):Square(sw, IID_STAR_SQUARE, x, y, right)
{
    
}

void StarSquare::doAction(PlayerAvatar* playerPtr)
{
    if(playerPtr->getCoins() < 20)
     return;
    
    playerPtr->setCoins(getWorld()->getPeach()->getCoins()-20);
    playerPtr->setStars(getWorld()->getPeach()->getStars()+1);
    getWorld()->playSound(SOUND_GIVE_STAR);
}

DirectionalSquare::DirectionalSquare(StudentWorld* sw, int x, int y, int dir):Square(sw, IID_DIR_SQUARE, x, y, dir)
{
    forcingDir = dir;
}

void DirectionalSquare::doAction(PlayerAvatar* playerPtr)
{
    playerPtr->setWalkDir(forcingDir);
}
