#include "Actor.h"
#include "StudentWorld.h"
#include "Board.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(StudentWorld* sw, int imgID, int x, int y, int dir = right, int depth = 0, double size = 1.0) : GraphObject(imgID, x, y, dir, depth, size), m_world(sw)
{
    alive = true;
}

//checks if position (x, y) is the same as the current position of this actor
bool Actor::isOn(int x, int y) const
{
    if(x == getX() && y == getY())
        return true;
    return false;
}

MovingActor::MovingActor(StudentWorld* sw, int imgID, int x, int y) : Actor(sw, imgID, x, y, right, 0, 1.0)
{
    ticks_to_move = 0;
    walkDir = right;
}

//gets the new direction of the actor and sets the walking direction to it (new direction may be the same as the old direction or a different one if actor is at a corner)
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
                //move down instead if possible
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
                //move down instead if possible
                else if(canMove(down, 2, x, y))
                    walkDir = down;
            }
            break;
        }
        case up:
        {
            if(!canMove(up, 2, x, y))
            {
                //move right instead if possible
                if(canMove(right, 2, x, y))
                    walkDir = right;
                //move left instead if possible
                else if(canMove(left, 2, x, y))
                    walkDir = left;
            }
            break;
        }
        case down:
        {
            if(!canMove(down, 2, x, y))
            {
                //move right instead if possible
                if(canMove(right, 2, x, y))
                    walkDir = right;
                //move left instead if possible
                else if(canMove(left, 2, x, y))
                    walkDir = left;
            }
            break;
        }
    }
}

//returns true if actor can move dist pixels in direction dir from position x, y, false otherwise
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

//if walking direction is left, sprite direction is set to 180, otherwise sprite direction is set to 0
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

//teleport actor to a random square, invalidates current walk direction to -1 and moves the player to new square
void MovingActor::teleportToRandomSquare()
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

PlayerAvatar::PlayerAvatar(StudentWorld* sw, int imgID, int x, int y, int playerNum):MovingActor(sw, imgID, x, y)
{
    playerNumber = playerNum;
    setState("waiting to roll");
    coins = 0;
    stars = 0;
}

//deals with the frame-by-frame action of the player
void PlayerAvatar::doSomething()
{
    if(getState() == "waiting to roll")
    {
        if(getWalkDir() == -1) //if walk dir was invalid due to being teleported
        {
            setWalkDir(chooseRandomWalkDir());
            updateSpriteDirection();
        }
        if(!canMove(getWalkDir(), 2, getX(), getY())) //if player is in a corner or cannot move in current direction
        {
            setWalkDir(chooseRandomWalkDir());
            updateSpriteDirection();
        }
        switch (getWorld()->getAction(playerNumber))
        {
            //action is pressed to move character
            case ACTION_ROLL:
            {
                int die_roll = randInt(1, 10);
                setTicksToMove(die_roll * 8);
                //setTicksToMove(8);
                setState("walking");
                break;
            }
            //action is pressed to fire vortex
            case ACTION_FIRE:
            {
                if(hasVortex)
                {
                    //create new vortex object and add it to the vector in the StudentWorld class
                    Vortex* vortex = new Vortex(getWorld(), getX(), getY());
                    vortex->setWalkDir(getWalkDir());
                    getWorld()->addNewActor(vortex);
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                    hasVortex = false;
                }
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
            
            //player is at a fork
            if(possibleDirs.size() > 2)
            {
                switch (getWorld()->getAction(playerNumber))
                {
                    case ACTION_LEFT:
                    {
                        //check if moving left is one of the possible actions
                        for(int i = 0; i < possibleDirs.size(); i++)
                        {
                            //if the player can move left and wasn't previously moving right (so that the player doesn't do a 180 turn)
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
            else //either continue in current direction or change direction if at a turn
                getNewDirection(getX(), getY());
        }

        updateSpriteDirection(); //change direction sprite is facing
        
        moveAtAngle(getWalkDir(), 2); //move character 2 pixels
        decTicksToMove(); //decrease ticks to move
       
        if(getTicksToMove() == 0)
        {
            getNewDirection(getX(), getY()); //to get new direction if character is at a turn and is stopping there
            setState("waiting to roll");
        }
    }
}

//swaps position, ticks to move, walking direction, sprite direction, and states with other
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

//swap stars with player pointed to by playerPtr
void PlayerAvatar::swapStars(PlayerAvatar* playerPtr)
{
    int temp = getStars();
    setStars(playerPtr->getStars());
    playerPtr->setStars(temp);
}

//swaps coins with player pointed to by playerPtr
void PlayerAvatar::swapCoins(PlayerAvatar* playerPtr)
{
    int temp = getCoins();
    setCoins(playerPtr->getCoins());
    playerPtr->setCoins(temp);
}

Enemy::Enemy(StudentWorld* sw, int imgID, int x, int y):MovingActor(sw, imgID, x, y)
{
    pauseCounter = 180;
    setState("Paused");
    setPeachIsNew(true);
    setYoshiIsNew(true);
}

//deals with frame-by-frame movement of enemy
void Enemy::doSomething()
{
    if(getState() == "Paused")
    {
        if(getWorld()->getPeach()->getState() == "waiting to roll")
        {
            if(isOn(getWorld()->getPeach()->getX(), getWorld()->getPeach()->getY()))
            {
                if(isPeachNew())
                {
                    doAction(getWorld()->getPeach()); //call action specific to Bowser or Boo on Peach if Peach is in waiting to roll state and is on same square as the enemy
                    setPeachIsNew(false); //set peach to not new
                }
            }
            else
                setPeachIsNew(true); //peach is no longer on the same square as this enemy, so she is new
        }
        
        if(getWorld()->getYoshi()->getState() == "waiting to roll")
        {
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
        
        pauseCounter--;
        if(pauseCounter == 0)
        {
            int die_roll = randInt(1, 10);
            setTicksToMove(die_roll * 8);
            
            std::vector<int> possibleDirs = getWorld()->getValidDirsFromPos(getX(), getY());
            int randDir = randInt(0, (int)possibleDirs.size() - 1);
            setWalkDir(possibleDirs[randDir]); //choose new random direction for enemy to move
            updateSpriteDirection();
            
            setState("Walking");
        }
    }
    if(getState() == "Walking")
    {
        std::vector<int> possibleDirs = getWorld()->getValidDirsFromPos(getX(), getY());
        
        //if enemy is at a fork
        if(possibleDirs.size() > 2)
        {
            //choose a random direction to move
            int randDir = randInt(0, (int)possibleDirs.size() - 1);
            setWalkDir(possibleDirs[randDir]);
        }
        else if(!canMove(getWalkDir(), 2, getX(), getY()))
            getNewDirection(getX(), getY()); //either continue moving in same direction or turn at a corner
        
        updateSpriteDirection();
        moveAtAngle(getWalkDir(), 2);
        decTicksToMove();
        if(getTicksToMove() == 0)
        {
            setState("Paused");
            pauseCounter = 180;
            if(canMakeDroppingSquare()) //if enemy is Bowser (can make dropping squares)
            {
                int randNum = randInt(1,4);
                if(randNum == 1) //25% of making dropping square
                {
                    getWorld()->replaceSquareWithDropping(getX(), getY());
                    getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
                }
            }
        }
    }
}

//called when an enemy is impacted (teleport to random square)
void Enemy::doImpactedAction()
{
    teleportToRandomSquare();
    setWalkDir(right);
    updateSpriteDirection();
    setState("Paused");
    pauseCounter = 180;
}

Bowser::Bowser(StudentWorld* sw, int x, int y):Enemy(sw, IID_BOWSER, x, y)
{
    
}

//do Bowser specific action when Bowser runs into a player
void Bowser::doAction(PlayerAvatar* playerPtr)
{
    int randNum = randInt(1,2);
    
    if(randNum == 1) //50% chance to set coins and stars of player to 0
    {
        playerPtr->setCoins(0);
        playerPtr->setStars(0);
        getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
    }
}

Boo::Boo(StudentWorld* sw, int x, int y):Enemy(sw, IID_BOO, x, y)
{
    
}

//do Boo specific action when Boo runs into a player
void Boo::doAction(PlayerAvatar* playerPtr)
{
    int randNum = randInt(1,2);
    
    if(randNum == 1) //50% chance to swap stars between the players
    {
        //is peach
        if(playerPtr->isPlayerOne())
            playerPtr->swapStars(getWorld()->getYoshi());
        //is yoshi
        else
            playerPtr->swapStars(getWorld()->getPeach());
    }
    else //50% chance to swap coins between the players
    {
        //is peach
        if(playerPtr->isPlayerOne())
            playerPtr->swapCoins(getWorld()->getYoshi());
        //is yoshi
        else
            playerPtr->swapCoins(getWorld()->getPeach());
    }
    getWorld()->playSound(SOUND_BOO_ACTIVATE);
}

Vortex::Vortex(StudentWorld* sw, int x, int y):MovingActor(sw, IID_VORTEX, x, y)
{
    
}

//do frame-by-frame action for vortex
void Vortex::doSomething()
{
    if(!isAlive())
        return;
    
    //move vortex 2 pixels in movement direction
    moveAtAngle(getWalkDir(), 2);
    
    //if Vortex has left boundaries
    if(getX() < 0 || getY() < 0 || getX() >= VIEW_WIDTH || getY() >= VIEW_HEIGHT)
        setIsAlive(false);
    
    Enemy* impactedActor = getWorld()->getOverlapEnemy(getX(), getY());
    //if there is an enemy that this vortex currently overlaps
    if(impactedActor != nullptr)
    {
        impactedActor->doImpactedAction(); //force impacted enemy to do impacted action
        setIsAlive(false); //deactivate vortex
        getWorld()->playSound(SOUND_HIT_BY_VORTEX);
    }
}

Square::Square(StudentWorld* sw, int imgID, int x, int y, int dir):Actor(sw, imgID, x, y, dir, 1, 1.0)
{
    setPeachIsNew(false);
    setYoshiIsNew(false);
}

//check if peach or yoshi has walked onto the square and is new
void Square::doSomething()
{
    if(!isAlive())
        return;
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

//if player lands on this square, add or subtract 3 coins depending on the type of coin
void CoinSquare::doAction(PlayerAvatar* playerPtr)
{
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

//if the player has less than 20 coins, return; otherwise, give the player 1 star in exchange for 20 coins
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

//set moving direction of player to the forcing direction
void DirectionalSquare::doAction(PlayerAvatar* playerPtr)
{
    playerPtr->setWalkDir(forcingDir);
}

BankSquare::BankSquare(StudentWorld* sw, int x, int y):Square(sw, IID_BANK_SQUARE, x, y, right)
{
    
}

//if player lands on this square, give the bank balance to the player; if the player moves over the square, subtract 5 coins from the player and place it into the bank
void BankSquare::doAction(PlayerAvatar* playerPtr)
{
    //if player lands on the square
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
        //if player has less than 5 coins, just give what the player has
        if(playerPtr->getCoins() < 5)
        {
            getWorld()->setBankBalance(getWorld()->getBankBalance() + playerPtr->getCoins());
            playerPtr->setCoins(0);
        }
        //give 5 coins to the bank
        else
        {
            playerPtr->setCoins(playerPtr->getCoins()-5);
            getWorld()->setBankBalance(getWorld()->getBankBalance() + 5);
        }

        getWorld()->playSound(SOUND_DEPOSIT_BANK);
    }
}

EventSquare::EventSquare(StudentWorld* sw, int x, int y):Square(sw, IID_EVENT_SQUARE, x, y, right)
{
    
}

//if the player lands on an event square and
void EventSquare::doAction(PlayerAvatar* playerPtr)
{
    if(playerPtr->getState() == "waiting to roll")
    {
        int randAction = randInt(1,3);
        //int randAction = 2;
        switch(randAction)
        {
            case 1:
            {
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
                getWorld()->playSound(SOUND_PLAYER_TELEPORT);
                break;
            }
            case 3:
            {
                playerPtr->setHasVortex(true);
                getWorld()->playSound(SOUND_GIVE_VORTEX);
                break;
            }
        }
    }
}

DroppingSquare::DroppingSquare(StudentWorld* sw, int x, int y):Square(sw, IID_DROPPING_SQUARE, x, y, right)
{
    
}

//if player lands on a dropping square, either subtract 10 coins or subtract one star
void DroppingSquare::doAction(PlayerAvatar* playerPtr)
{
    if(playerPtr->getState() == "waiting to roll")
    {
        int randAction = randInt(1,2);
        
        if(randAction == 1) //50% chance to remove 10 coins
        {
            playerPtr->setCoins(playerPtr->getCoins()-10);
            //if subtracted coins past 0
            if(playerPtr->getCoins() < 0)
                playerPtr->setCoins(0);
        }
        else //50% chance to remove 1 star
        {
            playerPtr->setStars(playerPtr->getStars()-1);
            //if subtracted stars past 0
            if(playerPtr->getStars() < 0)
                playerPtr->setStars(0);
        }
        getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
    }
}
