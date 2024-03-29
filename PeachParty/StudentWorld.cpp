#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    peach = nullptr;
    yoshi = nullptr;
    bankBalance = 0;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

//gets the board file to be played and loads it; returns board error if it cannot be loaded, otherwise it populates the board and starts the countdown timer
int StudentWorld::init()
{
    string boardFile = getBoardFile();
    Board bd;
    Board::LoadResult result = bd.loadBoard(boardFile);
    board = bd;
    
    if (result == Board::load_fail_file_not_found)
    {
        cerr << "Could not find board01.txt data file\n";
        return GWSTATUS_BOARD_ERROR;
    }
    else if (result == Board::load_fail_bad_format)
    {
        cerr << "Your board was improperly formatted\n";
        return GWSTATUS_BOARD_ERROR;
    }
    else if (result == Board::load_success) {
        cerr << "Successfully loaded board\n";
        populateBoard(board);
    }
   
	startCountdownTimer(99);
    return GWSTATUS_CONTINUE_GAME;
}

//gets the board file name depending on which board was chosen
string StudentWorld::getBoardFile()
{
    Board bd;
    string board_file;

    switch (getBoardNumber())
    {
        case 1:
        {
            board_file = assetPath() + "board01.txt";
            break;
        }
        case 2:
        {
            board_file = assetPath() + "board02.txt";
            break;
        }
        case 3:
        {
            board_file = assetPath() + "board03.txt";
            break;
        }
        case 4:
        {
            board_file = assetPath() + "board04.txt";
            break;
        }
        case 5:
        {
            board_file = assetPath() + "board05.txt";
            break;
        }
        case 6:
        {
            board_file = assetPath() + "board06.txt";
            break;
        }
        case 7:
        {
            board_file = assetPath() + "board07.txt";
            break;
        }
        case 8:
        {
            board_file = assetPath() + "board08.txt";
            break;
        }
        case 9:
        {
            board_file = assetPath() + "board09.txt";
            break;
        }
    }
    return board_file;
}

//creates the objects in the game and adds them to the vector actors
void StudentWorld::populateBoard(Board bd)
{
    for(int i = 0; i < BOARD_WIDTH; i++)
    {
        for(int j = 0; j < BOARD_HEIGHT; j++)
        {
            Board::GridEntry ge = bd.getContentsOf(i, j);
            switch (ge) {
                case Board::empty:
                //cout << "Location 5,10 is empty\n";
                break;
                case Board::boo:
                {
                    Actor* boo = new Boo(this, SPRITE_WIDTH*i, SPRITE_HEIGHT*j);
                    actors.push_back(boo);
                    
                    Actor* blueCoin = new CoinSquare(this, IID_BLUE_COIN_SQUARE, SPRITE_WIDTH*i, SPRITE_HEIGHT*j, true);
                    actors.push_back(blueCoin);
                    break;
                }
                case Board::bowser:
                {
                    Actor* bowser = new Bowser(this, SPRITE_WIDTH*i, SPRITE_HEIGHT*j);
                    actors.push_back(bowser);
                    
                    Actor* blueCoin = new CoinSquare(this, IID_BLUE_COIN_SQUARE, SPRITE_WIDTH*i, SPRITE_HEIGHT*j, true);
                    actors.push_back(blueCoin);
                    break;
                }
                case Board::player:
                {
                    peach = new PlayerAvatar(this, IID_PEACH, SPRITE_WIDTH*i, SPRITE_HEIGHT*j, 1);
                    
                    yoshi = new PlayerAvatar(this, IID_YOSHI, SPRITE_WIDTH*i, SPRITE_HEIGHT*j, 2);
                    
                    Actor* blueCoin = new CoinSquare(this, IID_BLUE_COIN_SQUARE, SPRITE_WIDTH*i, SPRITE_HEIGHT*j, true);
                    actors.push_back(blueCoin);
                    break;
                }
                //cout << "Location 5,10 has Peach & Yoshi and a blue coin square\n";
                case Board::red_coin_square:
                {
                    Actor* redCoin = new CoinSquare(this, IID_RED_COIN_SQUARE, SPRITE_WIDTH*i, SPRITE_HEIGHT*j, false);
                    actors.push_back(redCoin);
                    break;
                }
                case Board::blue_coin_square:
                {
                    Actor* blueCoin = new CoinSquare(this, IID_BLUE_COIN_SQUARE, SPRITE_WIDTH*i, SPRITE_HEIGHT*j, true);
                    actors.push_back(blueCoin);
                    break;
                }
                case Board::star_square:
                {
                    Actor* starSquare = new StarSquare(this, SPRITE_WIDTH*i, SPRITE_HEIGHT*j);
                    actors.push_back(starSquare);
                    break;
                }
                case Board::up_dir_square:
                {
                    Actor* upDirSquare = new DirectionalSquare(this, SPRITE_WIDTH*i, SPRITE_HEIGHT*j, 90);
                    actors.push_back(upDirSquare);
                    break;
                }
                case Board::down_dir_square:
                {
                    Actor* downDirSquare = new DirectionalSquare(this, SPRITE_WIDTH*i, SPRITE_HEIGHT*j, 270);
                    actors.push_back(downDirSquare);
                    break;
                }
                case Board::left_dir_square:
                {
                    Actor* leftDirSquare = new DirectionalSquare(this, SPRITE_WIDTH*i, SPRITE_HEIGHT*j, 180);
                    actors.push_back(leftDirSquare);
                    break;
                }
                case Board::right_dir_square:
                {
                    Actor* rightDirSquare = new DirectionalSquare(this, SPRITE_WIDTH*i, SPRITE_HEIGHT*j, 0);
                    actors.push_back(rightDirSquare);
                    break;
                }
                case Board::event_square:
                {
                    Actor* eventCoin = new EventSquare(this, SPRITE_WIDTH*i, SPRITE_HEIGHT*j);
                    actors.push_back(eventCoin);
                    break;
                }
                case Board::bank_square:
                {
                    Actor* bankCoin = new BankSquare(this, SPRITE_WIDTH*i, SPRITE_HEIGHT*j);
                    actors.push_back(bankCoin);
                    break;
                }
            }
        }
    }
}

int StudentWorld::move()
{
    //if game ended
    if (timeRemaining() <= 0)
    {
        playSound(SOUND_GAME_FINISHED);
        int winner = -1;
        //if peach has more stars than yoshi -> peach wins
        if(peach->getStars() > yoshi->getStars())
            winner = 1;
        //if yoshi has more stars than peach -> yoshi wins
        else if(peach->getStars() < yoshi->getStars())
            winner = 2;
        //if equal stars
        else
        {
            //if peach has more coins than yoshi -> peach wins
            if(peach->getCoins() > yoshi->getCoins())
                winner = 1;
            //if yoshi has more coins than peach -> yoshi wins
            else if(peach->getCoins() < yoshi->getCoins())
                winner = 2;
            //random winner
            else
                winner = randInt(1,2);
        }
        if(winner == 1)
        {
            setFinalScore(peach->getStars(), peach->getCoins());
            return GWSTATUS_PEACH_WON;
        }
        else
        {
            setFinalScore(yoshi->getStars(), yoshi->getCoins());
            return GWSTATUS_YOSHI_WON;
        }
    }
    
    //make all actors do something
    peach->doSomething();
    yoshi->doSomething();
    for(int i = 0; i < actors.size(); i++)
    {
        if(actors[i]->isAlive())
            actors[i]->doSomething();
        //if actor is dead, remove it from actors and delete it
        else
        {
            delete actors[i];
            actors.erase(actors.begin() + i);
        }
    }
    
    setGameStatText(getGameText());
    
	return GWSTATUS_CONTINUE_GAME;
}

//creates the game stat text shown at the top of the screen
string StudentWorld::getGameText()
{
    string gameText = "P1 Roll: ";
    gameText += std::to_string(peach->getTicksToMove()/8);
    gameText += " Stars: ";
    gameText += std::to_string(peach->getStars());
    gameText += " $$: ";
    gameText += std::to_string(peach->getCoins());
    if(peach->playerHasVortex())
        gameText += " VOR";
    gameText += " | Time: ";
    gameText += std::to_string(timeRemaining());
    gameText += " | Bank: ";
    gameText += std::to_string(bankBalance);
    gameText += " | P2 Roll: ";
    gameText += std::to_string(yoshi->getTicksToMove()/8);
    gameText += " Stars: ";
    gameText += std::to_string(yoshi->getStars());
    gameText += " $$: ";
    gameText += std::to_string(yoshi->getCoins());
    if(yoshi->playerHasVortex())
        gameText += " VOR";
    
    return gameText;
}

//deletes all actors at end of the game
void StudentWorld::cleanUp()
{
    while(!actors.empty())
    {
        delete actors[actors.size() - 1];
        actors.pop_back();
    }
    
    delete peach;
    peach = nullptr;
    
    delete yoshi;
    yoshi = nullptr;
}

//returns the grid entry for the contents at (x,y)
Board::GridEntry StudentWorld::getActorTypeAt(int x, int y)
{
    Board::GridEntry ge = board.getContentsOf(x, y);
    return ge;
}

//returns pointer to the actor at position (x,y) and its index in actors
Actor* StudentWorld::getSquareActorAt(int x, int y, int& index)
{
    for(int i = 0; i < actors.size(); i++)
    {
        if(actors[i]->getX() == x && actors[i]->getY() == y && actors[i]->isSquare())
        {
            index = i;
            return actors[i];
        }
    }
    index = -1;
    return nullptr;
}

//returns vector of integers with the valid directions an actor can move from (x,y)
vector<int> StudentWorld::getValidDirsFromPos(int x, int y)
{
    vector<int> validDirs;
    
    int i = 0;
    //left
    if(getSquareActorAt(x-SPRITE_WIDTH, y, i) != nullptr && getSquareActorAt(x-SPRITE_WIDTH, y, i)->isSquare())
        validDirs.push_back(180);
    //right
    if(getSquareActorAt(x+SPRITE_WIDTH, y, i) != nullptr && getSquareActorAt(x+SPRITE_WIDTH, y, i)->isSquare())
        validDirs.push_back(0);
    //up
    if(getSquareActorAt(x, y+SPRITE_HEIGHT, i) != nullptr && getSquareActorAt(x, y+SPRITE_HEIGHT, i)->isSquare())
        validDirs.push_back(90);
    //down
    if(getSquareActorAt(x, y-SPRITE_HEIGHT, i) != nullptr && getSquareActorAt(x, y-SPRITE_HEIGHT, i)->isSquare())
        validDirs.push_back(270);
    return validDirs;
}

//chooses random square on the board and returns pointer to it
Actor* StudentWorld::chooseRandomSquare() const
{
    //loop until we get an actor that is a squareS
    for(;;)
    {
        int randActor = randInt(0, (int)actors.size()-1);
        if(actors[randActor]->isSquare())
            return actors[randActor];
    }
    
    return nullptr;
}

//replaces square at (x,y) to a dropping square
void StudentWorld::replaceSquareWithDropping(int x, int y)
{
    int i = 0;
    Actor* square = getSquareActorAt(x, y, i);
    if(square != nullptr)
    {
        square->setIsAlive(false);
       
        square = new DroppingSquare(this, x, y);
        actors.push_back(square);
    }
}

//returns pointer to enemy that overlaps with a vortex at (x,y)
Enemy* StudentWorld::getOverlapEnemy(int x, int y)
{
    for(int i = 0; i < actors.size(); i++)
    {
        if(actors[i]->isImpactable())
        {
            if(x + SPRITE_WIDTH - 1 >= actors[i]->getX() && x < actors[i]->getX() + SPRITE_WIDTH)
            {
                if(y + SPRITE_HEIGHT - 1 >= actors[i]->getY() && y < actors[i]->getY() + SPRITE_HEIGHT)
                {
                    return static_cast<Enemy*>(actors[i]);
                }
            }
        }
    }
    return nullptr;
}

//adds a new actor (the parameter) to actors
void StudentWorld::addNewActor(Actor* actor)
{
    actors.push_back(actor);
}
