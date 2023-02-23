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
   //init();
}

int StudentWorld::init()
{
    string boardFile = getBoardFile();
    Board bd;
    Board::LoadResult result = bd.loadBoard(boardFile);
    
    if (result == Board::load_fail_file_not_found)
        cerr << "Could not find board01.txt data file\n";
    else if (result == Board::load_fail_bad_format)
        cerr << "Your board was improperly formatted\n";
    else if (result == Board::load_success) {
        cerr << "Successfully loaded board\n";
        populateBoard(bd);
    }
   
	startCountdownTimer(5);
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
                //cout << "Location 5,10 has a Boo and a blue coin square\n";
                break;
                case Board::bowser:
                //cout << "Location 5,10 has a Bowser and a blue coin square\n";
                break;
                case Board::player:
                //cout << "Location 5,10 has Peach & Yoshi and a blue coin square\n";
                break;
                case Board::red_coin_square:
                //cout << "Location 5,10 has a red coin square\n";
                break;
                case Board::blue_coin_square:
                {
                    Actor* blueCoin = new CoinSquare(this, SPRITE_WIDTH*i, SPRITE_HEIGHT*j);
                    actors.push_back(blueCoin);
                    cerr << actors.size() << endl;
                    break;
                }
            // etc…
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
    
    if (timeRemaining() <= 0)
        return GWSTATUS_PEACH_WON;
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete peach;
    
    while(!actors.empty())
    {
        //cerr << actors.size() << endl;
        delete actors[actors.size() - 1];
        //cerr << "hi" << endl;
        actors.pop_back();
        //cerr << "bye" << endl;
        //cerr << actors.size() << endl;
    }
}
