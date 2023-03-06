#include "Actor.h"
#include "StudentWorld.h"
#include "Board.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(StudentWorld* sw, int imgID, int x, int y, int dir = right, int depth = 0, double size = 1.0) : GraphObject(imgID, x, y, dir, depth, size), m_world(sw)
{
    alive = true;
}

bool Actor::isOn(int x, int y)
{
    if(x == getX() && y == getY())
        return true;
    return false;
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
        if(x-dist < 0 || getWorld()->getActorTypeAt((x-dist)/SPRITE_WIDTH, y/SPRITE_HEIGHT) == Board::empty)
            return false;
        return true;
    }
    else if(dir == right)
    {
        if(x+dist + SPRITE_WIDTH >= VIEW_WIDTH || getWorld()->getActorTypeAt(x/SPRITE_WIDTH+1, y/SPRITE_HEIGHT) == Board::empty)
            return false;
        return true;
    }
    else if(dir == up)
    {
        if(y+dist + SPRITE_HEIGHT >= VIEW_HEIGHT || getWorld()->getActorTypeAt(x/SPRITE_WIDTH, y/SPRITE_HEIGHT+1) == Board::empty)
            return false;
        return true;
    }
    else
    {
        if(y-dist <0 || getWorld()->getActorTypeAt(x/SPRITE_WIDTH, (y-dist)/SPRITE_HEIGHT) == Board::empty)
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
        if(getWalkDir() == -1) //if walk dir was invalid due to being teleported
        {
            setWalkDir(chooseRandomWalkDir());
            updateSpriteDirection();
        }
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
            case ACTION_FIRE:
            {
                
            }
            default:
                return;
        }
    }
    if(getState() == "walking")
    {
        //if player is not on directional square
        if(getWorld()->getActorTypeAt(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT) != Board::up_dir_square && getWorld()->getActorTypeAt(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT) != Board::down_dir_square && getWorld()->getActorTypeAt(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT) != Board::left_dir_square && getWorld()->getActorTypeAt(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT) != Board::right_dir_square)
        {
            std::vector<int> possibleDirs = getWorld()->getValidDirsFromPos(getX(), getY());
            if(possibleDirs.size() > 2)
            {
                switch (getWorld()->getAction(playerNumber))
                {
                    case ACTION_LEFT:
                    {
                        for(int i = 0; i < possibleDirs.size(); i++)
                        {
                            if(possibleDirs[i] == left && getWalkDir() != right)
                                setWalkDir(left);
                        }
                        break;
                    }
                    case ACTION_RIGHT:
                    {
                        for(int i = 0; i < possibleDirs.size(); i++)
                        {
                            if(possibleDirs[i] == right && getWalkDir() != left)
                                setWalkDir(right);
                        }
                        break;
                    }
                    case ACTION_UP:
                    {
                        for(int i = 0; i < possibleDirs.size(); i++)
                        {
                            if(possibleDirs[i] == up && getWalkDir() != down)
                                setWalkDir(up);
                        }
                        break;
                    }
                    case ACTION_DOWN:
                    {
                        for(int i = 0; i < possibleDirs.size(); i++)
                        {
                            if(possibleDirs[i] == down && getWalkDir() != up)
                                setWalkDir(down);
                        }
                        break;
                    }
                    default:
                        return;
                }
            }
            else
                getNewDirection(getX(), getY());
        }

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

void PlayerAvatar::teleportToRandomSquare()
{
    Actor* randSquare;
    for(;;)
    {
        randSquare = getWorld()->chooseRandomSquare();
        if(randSquare == nullptr) //meaning there are no other squares on the board
            return;
        //if the randomly chosen square is not the current square the player is on
        if(randSquare->getX() != getX() || randSquare->getY() != getY())
            break;
    }
    
    //invalidate walking dir
    setWalkDir(-1);
    
    moveTo(randSquare->getX(), randSquare->getY());
}

void PlayerAvatar::swapAttributesWithOther(PlayerAvatar* other)
{
    //swap positions
    int tempX = getX();
    int tempY = getY();
    
    moveTo(other->getX(), other->getY());
    other->moveTo(tempX, tempY);
    
    //swap ticks
    int tempTicks = getTicksToMove();
    setTicksToMove(other->getTicksToMove());
    other->setTicksToMove(tempTicks);
    
    //swap walkDirs
    int tempWalkDir = getWalkDir();
    setWalkDir(other->getWalkDir());
    other->setWalkDir(tempWalkDir);
    
    //update sprite directions
    updateSpriteDirection();
    other->updateSpriteDirection();
    
    //swap states
    std::string tempState = getState();
    setState(other->getState());
    other->setState(tempState);
}

void PlayerAvatar::swapStars(PlayerAvatar* playerPtr)
{
    int temp = getStars();
    setStars(playerPtr->getStars());
    playerPtr->setStars(temp);
}

void PlayerAvatar::swapCoins(PlayerAvatar* playerPtr)
{
    int temp = getCoins();
    setCoins(playerPtr->getCoins());
    playerPtr->setCoins(temp);
}

Enemy::Enemy(StudentWorld* sw, int imgID, int x, int y):MovingActor(sw, imgID, x, y, right, 0, 1.0)
{
    travelDist = 0;
    pauseCounter = 180;
    setState("Paused");
    setPeachIsNew(true);
    setYoshiIsNew(true);
}

void Enemy::doSomething()
{
    if(getState() == "Paused")
    {
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
        
        pauseCounter--;
        if(pauseCounter == 0)
        {
            int die_roll = randInt(1, 10);
            setTicksToMove(die_roll * 8);
            std::vector<int> possibleDirs = getWorld()->getValidDirsFromPos(getX(), getY());
            int randDir = randInt(0, (int)possibleDirs.size() - 1);
            setWalkDir(possibleDirs[randDir]);
            updateSpriteDirection();
            setState("Walking");
        }
    }
    if(getState() == "Walking")
    {
        std::vector<int> possibleDirs = getWorld()->getValidDirsFromPos(getX(), getY());
        if(possibleDirs.size() > 2)
        {
            int randDir = randInt(0, (int)possibleDirs.size() - 1);
            setWalkDir(possibleDirs[randDir]);
        }
        else if(!canMove(getWalkDir(), 2, getX(), getY()))
            getNewDirection(getX(), getY());
        
        updateSpriteDirection();
        moveAtAngle(getWalkDir(), 2);
        decTicksToMove();
        if(getTicksToMove() == 0)
        {
            setState("Paused");
            pauseCounter = 180;
            if(canMakeDroppingSquare())
            {
                int randNum = randInt(1,4);
                if(randNum == 1)
                {
                    getWorld()->replaceSquareWithDropping(getX(), getY());
                    getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
                }
            }
        }
    }
}

Bowser::Bowser(StudentWorld* sw, int x, int y):Enemy(sw, IID_BOWSER, x, y)
{
    
}

void Bowser::doAction(PlayerAvatar* playerPtr)
{
    int randNum = randInt(1,2);
    
    if(randNum == 1)
    {
        playerPtr->setCoins(0);
        playerPtr->setStars(0);
        getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
    }
}

Boo::Boo(StudentWorld* sw, int x, int y):Enemy(sw, IID_BOO, x, y)
{
    
}

void Boo::doAction(PlayerAvatar* playerPtr)
{
    int randNum = randInt(1,2);
    
    if(randNum == 1)
    {
        //is peach
        if(playerPtr->isPlayerOne())
            playerPtr->swapStars(getWorld()->getYoshi());
        else
            playerPtr->swapStars(getWorld()->getPeach());
    }
    else
    {
        //is peach
        if(playerPtr->isPlayerOne())
            playerPtr->swapCoins(getWorld()->getYoshi());
        else
            playerPtr->swapCoins(getWorld()->getPeach());
    }
    getWorld()->playSound(SOUND_BOO_ACTIVATE);
}

Square::Square(StudentWorld* sw, int imgID, int x, int y, int dir):Actor(sw, imgID, x, y, dir, 1, 1.0)
{
    setPeachIsNew(false);
    setYoshiIsNew(false);
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

BankSquare::BankSquare(StudentWorld* sw, int x, int y):Square(sw, IID_BANK_SQUARE, x, y, right)
{
    
}

void BankSquare::doAction(PlayerAvatar* playerPtr)
{
    if(playerPtr->getState() == "waiting to roll")
    {
        if(getWorld()->getBankBalance() > 0)
        {
            playerPtr->setCoins(playerPtr->getCoins()+getWorld()->getBankBalance());
            getWorld()->setBankBalance(0);
            
            getWorld()->playSound(SOUND_WITHDRAW_BANK);
        }
    }
    else
    {
        playerPtr->setCoins(playerPtr->getCoins()-5);
        //if subtracted coins past 0
        if(playerPtr->getCoins() < 0)
            playerPtr->setCoins(0);
        
        getWorld()->playSound(SOUND_DEPOSIT_BANK);
    }
}

EventSquare::EventSquare(StudentWorld* sw, int x, int y):Square(sw, IID_EVENT_SQUARE, x, y, right)
{
    
}

void EventSquare::doAction(PlayerAvatar* playerPtr)
{
    if(playerPtr->getState() == "waiting to roll")
    {
        int randAction = randInt(1,3);
        
        switch(randAction)
        {
            case 1:
            {
                std::cerr << "teleport to random square" << std::endl;
                playerPtr->teleportToRandomSquare();
                getWorld()->playSound(SOUND_PLAYER_TELEPORT);
                break;
            }
            case 2: //fixxxxx
            {
                //if player is peach
                if(playerPtr->isPlayerOne())
                {
                    playerPtr->swapAttributesWithOther(getWorld()->getYoshi());
                    setYoshiIsNew(false);
                }
                //if player is yoshi
                if(!playerPtr->isPlayerOne())
                {
                    playerPtr->swapAttributesWithOther(getWorld()->getPeach());
                    setPeachIsNew(false);
                }
                std::cerr << "swap with other player" << std::endl;
                getWorld()->playSound(SOUND_PLAYER_TELEPORT);
                break;
            }
            case 3:
            {
                playerPtr->setHasVortex(true);
                getWorld()->playSound(SOUND_GIVE_VORTEX);
                std::cerr << "give vortex" << std::endl;
                break;
            }
        }
    }
}

DroppingSquare::DroppingSquare(StudentWorld* sw, int x, int y):Square(sw, IID_DROPPING_SQUARE, x, y, right)
{
    
}

void DroppingSquare::doAction(PlayerAvatar* playerPtr)
{
    if(playerPtr->getState() == "waiting to roll")
    {
        int randAction = randInt(1,2);
        
        if(randAction == 1)
        {
            playerPtr->setCoins(playerPtr->getCoins()-10);
            //if subtracted coins past 0
            if(playerPtr->getCoins() < 0)
                playerPtr->setCoins(0);
        }
        else
        {
            playerPtr->setStars(playerPtr->getStars()-1);
            //if subtracted stars past 0
            if(playerPtr->getStars() < 0)
                playerPtr->setStars(0);
        }
        getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
    }
}
