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
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    string boardFile = getBoardFile();
    Board bd;
    Board::LoadResult result = bd.loadBoard(boardFile);
    board = bd;
    
    if (result == Board::load_fail_file_not_found)
        cerr << "Could not find board01.txt data file\n";
    else if (result == Board::load_fail_bad_format)
        cerr << "Your board was improperly formatted\n";
    else if (result == Board::load_success) {
        cerr << "Successfully loaded board\n";
        populateBoard(board);
    }
   
	startCountdownTimer(99);
    return GWSTATUS_CONTINUE_GAME;
}

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
                    //add Boo
                    
                    
                    Actor* blueCoin = new CoinSquare(this, IID_BLUE_COIN_SQUARE, SPRITE_WIDTH*i, SPRITE_HEIGHT*j, true);
                    actors.push_back(blueCoin);
                    break;
                }
                break;
                case Board::bowser:
                //cout << "Location 5,10 has a Bowser and a blue coin square\n";
                break;
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
                    Actor* blueCoin = new CoinSquare(this, IID_BLUE_COIN_SQUARE, SPRITE_WIDTH*i, SPRITE_HEIGHT*j, true);
                    actors.push_back(blueCoin);
                    break;
                }
                case Board::bank_square:
                {
                    Actor* blueCoin = new CoinSquare(this, IID_BLUE_COIN_SQUARE, SPRITE_WIDTH*i, SPRITE_HEIGHT*j, true);
                    actors.push_back(blueCoin);
                    break;
                }
            }
        }
    }
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.

    //setGameStatText("Game will end in a few seconds");
    
    //if (timeRemaining() <= 0)
		//return GWSTATUS_NOT_IMPLEMENTED;
    peach->doSomething();
    yoshi->doSomething();
    for(int i = 0; i < actors.size(); i++)
    {
        if(actors[i]->isAlive())
            actors[i]->doSomething();
    }
    
   // if (timeRemaining() <= 0)
     //   return GWSTATUS_PEACH_WON;
    
    //remove dead actors and vortex
    
    //display status
    
	return GWSTATUS_CONTINUE_GAME;
}

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

Board::GridEntry StudentWorld::getActorTypeAt(int x, int y)
{
    Board::GridEntry ge = board.getContentsOf(x, y);
    return ge;
}

Actor* StudentWorld::getActorAt(int x, int y)
{
    for(int i = 0; i < actors.size(); i++)
    {
        if(actors[i]->getX() == x && actors[i]->getY() == y)
            return actors[i];
    }
    return nullptr;
}

vector<int> StudentWorld::getValidDirsFromPos(int x, int y)
{
    vector<int> validDirs;
    
    //left
    if(getActorAt(x-SPRITE_WIDTH, y) != nullptr && getActorAt(x-SPRITE_WIDTH, y)->isSquare())
        validDirs.push_back(180);
    //right
    if(getActorAt(x+SPRITE_WIDTH, y) != nullptr && getActorAt(x+SPRITE_WIDTH, y)->isSquare())
        validDirs.push_back(0);
    //up
    if(getActorAt(x, y+SPRITE_HEIGHT) != nullptr && getActorAt(x, y+SPRITE_HEIGHT)->isSquare())
        validDirs.push_back(90);
    //left
    if(getActorAt(x, y-SPRITE_HEIGHT) != nullptr && getActorAt(x, y-SPRITE_HEIGHT)->isSquare())
        validDirs.push_back(270);
    return validDirs;
}
